/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:21:50 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/06 19:43:21 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_pipe(t_ast *root, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini *last;
	pid_t pid;
	int status;
	int pipe_fd[2];
	int prev_fd;
	int redir_fd;
	t_env *e_status;
	char **envp;
	char *file_heredoc;
	int error;

	redir_fd = -1;
	status = -1;
	error = 0;
	file_heredoc = NULL;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	// gestion des quotes et expand
	if (root->token->type == T_CMD && root->token->cmd)
	{
		if ((root->token->cmd->cmd && root->token->cmd->args) || (!root->token->cmd->cmd && *root->token->cmd->args))
			handle_expand(root, last);
		// premier appel de la fonction pour verifier les file
		if (root->token->cmd->redir && root->token->cmd->redir->type != REDIR_HEREDOC)
			redir_fd = ft_handle_redir_file(redir_fd, root, last);
		// cas de redirection pour "cat file" sans sympbole de redirection
		else if (!ft_strcmp(root->token->cmd->args[0], "cat") && !root->token->cmd->redir && redir_fd == -1 && root->token->cmd->args[1])
		{
			redir_fd = open(root->token->cmd->args[1], O_RDONLY);
			if (redir_fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(root->token->cmd->args[1], 2);
				ft_putendl_fd(": No such file or directory", 2);
				redir_fd = 5;
			}
		}
		// si erreur de file, on execute pas le reste des commandes
		if (redir_fd == 5 || (root->token->cmd->redir && root->token->cmd->redir->type != REDIR_HEREDOC && !root->token->cmd->cmd))
		{
			printf("if fd == 5 -> error == 1\n");
			e_status = ft_get_exit_status(&last->env);
			if (e_status)
				ft_change_exit_status(e_status, ft_itoa(1));
			else
				ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(1)));
			error = 1;
		}
		ft_set_var_underscore(root->token->cmd->args, &last->env, envp);
		// si pas de file de redirection, on redirige vers la sortie standard
		if (redir_fd == -1)
		{
			printf("if no file -> redir = STDOUT_FILENO\n");
			redir_fd = STDOUT_FILENO;
		}
		if (redir_fd != STDOUT_FILENO && redir_fd != -1)
		{
			printf("fermutre du redir_fd = %d\n", redir_fd);
			close(redir_fd);
			printf(" after close -> redir_fd = %d\n", redir_fd);
		}
		// si pas d'erreur, on execute la commande
		if (!error)
		{
			if (ft_is_builtin(root->token->cmd->cmd))
			{
				status = ft_exec_builtin(root->token, &last->env, redir_fd);
				if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
					close(redir_fd);
				ft_free_tab(envp);
				envp = NULL;
			}
			else if (!ft_strcmp(root->token->cmd->cmd, "exit"))
			{
				status = ft_exit(root, mini, prompt, envp);
				e_status = ft_get_exit_status(&last->env);
				if (e_status)
					ft_change_exit_status(e_status, ft_itoa(status));
				else
					ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(status)));
				ft_free_tab(envp);
				envp = NULL;
			}
			else
			{
				ft_get_signal_cmd();
				printf("lancement de fork\n");
				redir_fd = -1;
				if (pipe(pipe_fd) < 0)
					return (ft_putendl_fd("minishell: pipe failed", 2), 1);
				pid = fork();
				if (pid < 0)
					return (ft_putendl_fd("minishell: fork failed", 2), 1);
				if (pid == 0)
				{
					close(pipe_fd[0]);
					// deuxieme appel de la fonction pour verifier les file et here_doc
					if (root->token->cmd->redir)
						redir_fd = ft_handle_redir_file(redir_fd, root, last);
					if ((redir_fd != -1 && redir_fd != STDOUT_FILENO))
					{
						if (root->token->cmd->redir->type == REDIR_INPUT)
							dup2(redir_fd, STDIN_FILENO);
						else if (root->token->cmd->redir->type == REDIR_OUTPUT)
							dup2(redir_fd, STDOUT_FILENO);
						else if (root->token->cmd->redir->type == REDIR_APPEND)
							dup2(redir_fd, STDOUT_FILENO);
						else if (root->token->cmd->redir->type == REDIR_HEREDOC)
							dup2(redir_fd, STDIN_FILENO);
						close(redir_fd);
					}
					// cas de redirection pour "cat file" sans sympbole de redirection
					else if (!ft_strcmp(root->token->cmd->args[0], "cat") && redir_fd == -1 && root->token->cmd->args[1])
					{
						redir_fd = open(root->token->cmd->args[1], O_RDONLY);
						if (redir_fd < 0)
						{
							ft_putstr_fd("minishell: ", 2);
							ft_putstr_fd(root->token->cmd->args[1], 2);
							ft_putendl_fd(": No shuch file or directory", 2);
							ft_free_tab(envp);
							ft_clear_lst(mini);
							if (granny)
								ft_clear_ast(granny);
							free(prompt);
							exit(EXIT_FAILURE);
						}
						dup2(redir_fd, STDIN_FILENO);
						close(redir_fd);
					}
					else
					{
						close(pipe_fd[0]);
						dup2(pipe_fd[1], STDOUT_FILENO);
						close(pipe_fd[1]);
					}
					if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
						close(redir_fd);
					// execution de la commande
					ft_exec_cmd_path(root, granny, mini, envp, prompt);
					exit(EXIT_SUCCESS);
				}
				else
				{
					prev_fd = pipe_fd[0];
					if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
						close(redir_fd);
					if (file_heredoc)
						free(file_heredoc);
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))
					{
						e_status = ft_get_exit_status(&last->env);
						if (e_status)
							ft_change_exit_status(e_status, ft_itoa(WEXITSTATUS(status)));
						else
							ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(WEXITSTATUS(status))));
						ft_free_tab(envp);
						return (WEXITSTATUS(status));
					}
				}
			}
		}
	}
	if (envp)
		ft_free_tab(envp);
	return (status);
}
// int	ft_exec_cmd_pipe(t_ast *root, t_mini **mini, char *prompt)
// {
// 	t_mini	*last;
// 	t_env	*e_status;
// 	pid_t	pid;
// 	char	**envp;
// 	int		status;
// 	int		redir_fd;
// 	int		pipe_fd[2];
// 	int		prev_fd;

// 	redir_fd = -1;
// 	status = -1;
// 	redir_fd = ft_handle_redir_file(redir_fd, root);
// 	last = ft_minilast(*mini);
// 	envp = ft_get_envp(&last->env);
// 	if (!envp)
// 		return (ft_putendl_fd("minishell: malloc failed", 2), 1);
// 	if (root->token->type == T_CMD)
// 	{
// 		if (pipe(pipe_fd) < 0)
// 			return (ft_putendl_fd("minishell: pipe failed", 2), 1);
// 		ft_set_var_underscore(root->token->cmd->args, &last->env, envp);
// 		if (redir_fd == -1)
// 			redir_fd = STDOUT_FILENO;
// 		else
// 		{
// 			pid = fork();
// 			if (pid < 0)
// 				return (ft_putendl_fd("minishell: fork failed", 2), 1);
// 			if (pid == 0)
// 			{
// 				close(pipe_fd[0]);
// 				dup2(pipe_fd[1], redir_fd);
// 				close(pipe_fd[1]);
// 				if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
// 				{
// 					if (root->token->cmd->redir->type == REDIR_INPUT)
// 						dup2(redir_fd, STDIN_FILENO);
// 					else if (root->token->cmd->redir->type == REDIR_OUTPUT)
// 						dup2(redir_fd, STDOUT_FILENO);
// 					close(redir_fd);
// 				}
// 				if (ft_is_builtin(root->token->cmd->cmd))
// 				{
// 					status = ft_exec_builtin(root->token, &last->env, redir_fd);
// 					ft_free_tab(envp);
// 					exit(status);
// 				}
// 				else
// 				{
// 					ft_exec_cmd_path(root, mini, envp, prompt);
// 					exit(EXIT_SUCCESS);
// 				}
// 			}
// 			else
// 			{
// 				if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
// 					close(redir_fd);
// 				close(pipe_fd[1]);
// 				waitpid(pid, &status, 0);
// 				if (WIFEXITED(status))
// 				{
// 					e_status = ft_get_exit_status(&last->env);
// 					if (e_status)
// 						ft_change_exit_status(e_status,
// 												ft_itoa(WEXITSTATUS(status)));
// 					else
// 						ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"),
// 									ft_itoa(WEXITSTATUS(status))));
// 					ft_free_tab(envp);
// 					return (WEXITSTATUS(status));
// 				}
// 			}
// 		}
// 	}
// 	if (envp)
// 		ft_free_tab(envp);
// 	return (status);
// }
