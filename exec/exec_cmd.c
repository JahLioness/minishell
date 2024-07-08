/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/06 19:41:49 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// cat >file = comme un heredoc on ecrit dans le file jusqu'a ce qu'on fasse ctrl + D ou ctrl + C ou ctrl + "\""
//  ls && ls - l (echo hey || echo bye) <-- syntaxe error non geree
// je dois paufiner heredoc --> randomiser le nom de fichier, le faire executer en premier si il y a d'autre commande avant
// les pipes et la norminette a faire.
// je vais changer les gestion du heredoc, a part des files. 
// si MINI->heredoc = 1,
// creation du here_doc
// si plusieurs heredoc, (token->redir->next). recreer un nouveau fichier a chaque fois (mais la creation du fichier se fait dans le parent et l'ouverture dans le fils)
// a anticiper cat devant plusieurs heredoc
// probleme expand --> echo $USER "$USER" $USER

int ft_exec_builtin(t_token *token, t_env **env, int fd)
{
	t_env *status;
	int exit_status;

	exit_status = -1;
	if (!ft_strcmp(token->cmd->cmd, "echo"))
		exit_status = ft_echo(fd, ft_get_args_echo(token->cmd->args, env), ft_get_flag_echo(token->cmd->args));
	else if (!ft_strcmp(token->cmd->cmd, "cd"))
		exit_status = ft_cd(token->cmd->args, env);
	else if (!ft_strcmp(token->cmd->cmd, "pwd"))
		exit_status = ft_pwd(fd, env);
	else if (!ft_strcmp(token->cmd->cmd, "export"))
		exit_status = ft_exec_export(token, env, fd);
	else if (!ft_strcmp(token->cmd->cmd, "unset"))
		exit_status = ft_exec_unset(token->cmd, env);
	else if (!ft_strcmp(token->cmd->cmd, "env"))
		exit_status = ft_print_env(env, fd);
	status = ft_get_exit_status(env);
	if (status && exit_status != -1)
		ft_change_exit_status(status, ft_itoa(exit_status));
	else if (exit_status != -1)
		ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(exit_status)));
	return (exit_status);
}

int ft_exec_cmd_path(t_ast *root, t_ast *granny, t_mini **mini, char **envp, char *prompt)
{
	char *cmd_path;
	t_mini *last;

	last = ft_minilast(*mini);
	if (!root->token->cmd->cmd)
	{
		ft_free_tab(envp);
		ft_clear_lst(mini);
		if (root)
			ft_clear_ast(root);
		free(prompt);
		return (0);
	}
	if (!ft_strcmp(root->token->cmd->cmd, "$_"))
		cmd_path = ft_tabchr(envp, "_", '=');
	else if (ft_strchr(root->token->cmd->cmd, '/'))
		cmd_path = ft_strdup(root->token->cmd->cmd);
	else
		cmd_path = ft_get_cmd_path_env(root->token->cmd->cmd, envp);
	if (!cmd_path)
	{
		if (execve(root->token->cmd->cmd, root->token->cmd->args, envp) == -1)
			ft_exec_cmd_error(granny, mini, envp, prompt);
	}
	else
	{
		if (execve(cmd_path, root->token->cmd->args, envp) == -1)
		{
			free(cmd_path);
			ft_exec_cmd_error(granny, mini, envp, prompt);
		}
	}
	return (0);
}

int ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

int ft_handle_redir_file(int redir_fd, t_ast *root, t_mini *last)
{
	t_redir *current;
	char *file_heredoc;

	file_heredoc = NULL;
	current = root->token->cmd->redir;
	while (current)
	{
		printf("redir_fd in (while current) = %d\n", redir_fd);
		if (current->type == REDIR_INPUT)
		{
			if (redir_fd != -1)
			{
				printf("fermeture du premier redir_fd = %d\n", redir_fd);
				close(redir_fd);
			}
			redir_fd = open(current->file, O_RDONLY);
			if (redir_fd < 0)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(current->file, 2), ft_putendl_fd(": No such file or directory", 2), 5);
		}
		else if (current->type == REDIR_OUTPUT)
		{
			if (redir_fd != -1)
			{
				printf("fermeture du premier redir_fd = %d\n", redir_fd);
				close(redir_fd);
			}
			redir_fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (redir_fd < 0)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(current->file, 2), ft_putendl_fd(": No such file or directory", 2), 5);
		}
		else if (current->type == REDIR_APPEND)
		{
			if (redir_fd != -1)
			{
				printf("fermeture du premier redir_fd = %d\n", redir_fd);
				close(redir_fd);
			}
			redir_fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (redir_fd < 0)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(current->file, 2), ft_putendl_fd(": No such file or directory", 2), 5);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			if (redir_fd != -1)
				close(redir_fd);
			//creation du file heredoc && lancement du prompt
			file_heredoc = ft_get_heredoc(current, root, last);
			redir_fd = open(file_heredoc, O_RDONLY);
			if (redir_fd < 0)
			{
				unlink(file_heredoc);
				free(file_heredoc);
				return (5);
			}
			unlink(file_heredoc);
			free(file_heredoc);
			// close(redir_fd);
		}
		current = current->next;
	}
	return (redir_fd);
}

void handle_expand(t_ast *root, t_mini *last)
{
	int i;

	i = 0;
	while (root->token->cmd->args[i])
	{
		root->token->cmd->args[i] = ft_verif_arg(root->token->cmd->args, &last->env, root->token->cmd, i);
		i++;
	}
	root->token->cmd->args = ft_trim_quote_args(root->token->cmd->args);
	if (root->token->cmd->cmd)
		free(root->token->cmd->cmd);
	root->token->cmd->cmd = ft_strdup(root->token->cmd->args[0]);
}
int ft_exec_cmd(t_ast *root, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini *last;
	pid_t pid;
	int status;
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
				pid = fork();
				if (pid < 0)
					return (ft_putendl_fd("minishell: fork failed", 2), 1);
				if (pid == 0)
				{
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
					if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
						close(redir_fd);
					// execution de la commande
					ft_exec_cmd_path(root, granny, mini, envp, prompt);
					exit(EXIT_SUCCESS);
				}
				else
				{
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

// if (g_sig == 3)
// {
// 	ft_putendl_fd("Quit (core dumped)", 2);
// 	g_sig = 0;
// 	e_status = ft_get_exit_status(&last->env);
// 	if (e_status)
// 		ft_change_exit_status(e_status, ft_itoa(WEXITSTATUS(1)));
// 	else
// 		ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(WEXITSTATUS(1))));
// 	ft_free_tab(envp);
// }