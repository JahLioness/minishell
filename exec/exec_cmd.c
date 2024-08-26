/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/26 18:56:31 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(t_ast *root, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini	*last;
	t_env	*e_status;
	t_exec	*exec;
	t_cmd	*cmd;
	// t_cmd	*tmp_cmd;
	t_redir	*tmp;
	char	**envp;
	int		len_cmd;
	int		i;

	i = -1;
	tmp = NULL;
	e_status = NULL;
	cmd = root->token->cmd;
	exec = cmd->exec;
	if (cmd->redir)
		tmp = cmd->redir;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	len_cmd = ft_cmdsize(cmd);
	// tmp_cmd = cmd;
	// gestion des quotes et expand
	if (root->token->type == T_CMD && root->token->cmd)
	{
		if ((cmd->cmd && cmd->args) || (!cmd->cmd && *cmd->args))
			handle_expand(cmd, last);
		// premier appel de la fonction pour verifier les file
		if (cmd->redir && cmd->redir->type != REDIR_HEREDOC)
			handle_redir(cmd, mini, e_status);
		ft_set_var_underscore(cmd->args, &last->env, envp);
		// si pas d'erreur, on execute la commande
		if (!exec->error_ex)
		{
			//FAIRE MODIFICTION SUR LES FICHIER DE REDICTION POUR LES BUILTINS
			if (len_cmd >  1)
			{
				ft_get_signal_cmd();
				reset_fd(exec);
				while (++i < len_cmd)
				{
					// printf("IN BOUCLE tmp_cmd->cmd = %s\n", cmd->cmd);
					exec = cmd->exec;
					if (pipe(exec->pipe_fd) < 0)
						return (ft_putendl_fd("minishell: pipe failed", 2), 1);
					if (ft_is_builtin(cmd->cmd))
					{
						handle_builtin(cmd, last, tmp, exec);
						// if (cmd->redir)
						// {
						// 	printf("BUILT cmd->redir->file = %s\n", cmd->redir->file);
						// }
						// printf("BUILT cmd->exec->redir_in = %d\n", cmd->exec->redir_in);
						// printf("BUILT cmd->exec->redir_out = %d\n", cmd->exec->redir_out);
						// printf("BUILT exec->pipe_fd[0] = %d\n", exec->pipe_fd[0]);
						// printf("BUILT exec->pipe_fd[1] = %d\n", exec->pipe_fd[1]);
						// printf("BUILT exec->prev_fd = %d\n", exec->prev_fd);
						if (cmd->next)
						{
							if (exec->prev_fd != -1)
								close(cmd->next->exec->prev_fd);
							close(exec->pipe_fd[1]);
							// printf("NEXT exec->pipe_fd[0] = %d\n", cmd->next->exec->pipe_fd[0]);
							// printf("NEXT exec->pipe_fd[1] = %d\n", cmd->next->exec->pipe_fd[1]);
							// printf("NEXT exec->prev_fd = %d\n", cmd->next->exec->prev_fd);
							// printf("NEXT cmd->next->exec->prev_fd = %d\n", cmd->next->exec->prev_fd);
							cmd->next->exec->prev_fd = exec->pipe_fd[0];
						}
					}	
					else if (!ft_strcmp(cmd->cmd, "exit"))
					{
						if (envp)
							ft_free_tab(envp);
						envp = NULL;
						handle_exit(root, mini, e_status, prompt);
					}
					else
					{
						exec->pid = fork();
						if (exec->pid < 0)
							return (ft_putendl_fd("minishell: fork failed", 2), 1);
						if (exec->pid == 0)
						{
							if (cmd->redir)
							{
								// handle_redir(cmd, mini, e_status);
								handle_redir_dup(exec, cmd);
							}
							// if (cmd->redir)
							// 	printf("cmd->redir->file = %s\n", cmd->redir->file);
							// printf("cmd->exec->redir_in = %d\n", cmd->exec->redir_in);
							// printf("cmd->exec->redir_out = %d\n", cmd->exec->redir_out);
							ft_free_tab(envp);
							envp = NULL;
							process_child(cmd, i, len_cmd);
							reset_fd(exec);
							exec_command(cmd, granny, mini, prompt);
							exit(EXIT_SUCCESS);
						}
						else
						{
							// printf("exec->pipe_fd[0] = %d\n", exec->pipe_fd[0]);
							// printf("exec->pipe_fd[1] = %d\n", exec->pipe_fd[1]);
							// printf("exec->prev_fd = %d\n", exec->prev_fd);
							if (exec->prev_fd != -1)
								close(cmd->exec->prev_fd);
							if (cmd->next)
							{
								close(exec->pipe_fd[1]);
								// printf("NEXT exec->pipe_fd[0] = %d\n", cmd->next->exec->pipe_fd[0]);
								// printf("NEXT exec->pipe_fd[1] = %d\n", cmd->next->exec->pipe_fd[1]);
								// printf("NEXT exec->prev_fd = %d\n", cmd->next->exec->prev_fd);
								// printf("NEXT cmd->next->exec->prev_fd = %d\n", cmd->next->exec->prev_fd);
								cmd->next->exec->prev_fd = exec->pipe_fd[0];
							}
						}
					}
					reset_fd(exec);
					cmd = cmd->next;
				}
				if (envp)
					ft_free_tab(envp);
				envp = NULL;
				// root->token->cmd = tmp_cmd;
				close_fd(exec->pipe_fd, exec->prev_fd);
				waitpid(exec->pid, &exec->status, 0);
				return (handle_sigint(exec, last, e_status));
			}
			else if (len_cmd == 1)
			{
				ft_get_signal_cmd();
				reset_fd(exec);
				if (ft_is_builtin(cmd->cmd))
				{
					// printf("2\n\n");
					handle_builtin(cmd, last, tmp, exec);
				}
				else if (!ft_strcmp(cmd->cmd, "exit"))
				{
					if (envp)
						ft_free_tab(envp);
					envp = NULL;
					handle_exit(root, mini, e_status, prompt);
				}
				else
				{
					exec->pid = fork();
					if (exec->pid < 0)
						return (ft_putendl_fd("minishell: fork failed", 2), 1);
					if (exec->pid == 0)
					{
						// deuxieme appel de la fonction pour verifier les file et here_doc
						if (cmd->redir)
							handle_redir_dup(exec, cmd);
						reset_fd(exec);
						ft_free_tab(envp);
						exec_command(cmd, granny, mini, prompt);
						exit(EXIT_SUCCESS);
					}
					else
					{
						if (envp)
							ft_free_tab(envp);
						reset_fd(exec);
						waitpid(exec->pid, &exec->status, 0);
						return (handle_sigint(exec, last, e_status));
					}
				}
			}
		}
	}
	e_status = ft_get_exit_status(&last->env);
	handle_sigquit(envp, exec, e_status);
	if (envp)
		ft_free_tab(envp);
	// close_fd(exec->pipe_fd, exec->prev_fd);
	return (exec->status);
}
