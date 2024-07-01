/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 13:21:50 by andjenna          #+#    #+#             */
/*   Updated: 2024/06/27 19:15:38 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

// int	ft_exec_cmd_pipe(t_ast *root, t_mini **mini, char *prompt)
// {
// 	t_mini	*last;
// 	t_env	*e_status;
// 	pid_t	pid;
// 	char	**envp;
// 	int		status;
// 	int		redir_fd;
// 	int		pipe_fd[2];
// 	// int		prev_fd;

// 	redir_fd = -1;
// 	status = -1;
// 	redir_fd = ft_handle_redir_file(redir_fd, root);
// 	last = ft_minilast(*mini);
// 	envp = ft_get_envp(&last->env);
// 	if (!envp)
// 		return (ft_putendl_fd("minishell: malloc failed", 2), 1);
// 	if (!root->token || !root->token->cmd || !root->token->cmd->cmd)
// 		return (ft_free_tab(envp), 0);
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
// 				dup2(pipe_fd[1], STDOUT_FILENO);
// 				close(pipe_fd[1]);
// 				if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
// 				{
// 					if (root->token->cmd->redir->type == REDIR_INPUT)
// 						dup2(redir_fd, STDIN_FILENO);
// 					else if (root->token->cmd->redir->type == REDIR_OUTPUT)
// 						dup2(redir_fd, STDOUT_FILENO);
// 					close(redir_fd);
// 				}
// 				// else if (redir_fd == -1 && root->token->cmd->args[1])
// 				// {
// 				// 	redir_fd = open(root->token->cmd->args[1], O_RDONLY);
// 				// 	if (redir_fd < 0)
// 				// 	{
// 				// 		perror("open");
// 				// 		exit(EXIT_FAILURE);
// 				// 	}
// 				// 	dup2(redir_fd, STDIN_FILENO);
// 				// }
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
