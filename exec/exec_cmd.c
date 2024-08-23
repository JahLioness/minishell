/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/23 19:46:58 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_builtin(t_cmd *cmd, t_env **env, int fd)
{
	t_env	*status;
	int		exit_status;

	exit_status = 0;
	if (!ft_strcmp(cmd->cmd, "echo"))
		exit_status = ft_echo(fd, ft_get_args_echo(cmd->args, env),
				ft_get_flag_echo(cmd->args));
	else if (!ft_strcmp(cmd->cmd, "cd"))
		exit_status = ft_cd(cmd->args, env);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		exit_status = ft_pwd(fd, env);
	else if (!ft_strcmp(cmd->cmd, "export"))
		exit_status = ft_exec_export(cmd, env, fd);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		exit_status = ft_exec_unset(cmd, env);
	else if (!ft_strcmp(cmd->cmd, "env"))
		exit_status = ft_print_env(env, fd);
	status = ft_get_exit_status(env);
	ft_change_exit_status(status, ft_itoa(exit_status));
	return (exit_status);
}

int	ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

void	ft_handle_empty_first_arg(t_ast *root)
{
	int	i;

	i = 0;
	if (root->token->cmd->cmd)
		free(root->token->cmd->cmd);
	root->token->cmd->cmd = ft_strdup(root->token->cmd->args[1]);
	i = 0;
	while (root->token->cmd->args[i + 1])
	{
		free(root->token->cmd->args[i]);
		root->token->cmd->args[i] = ft_strdup(root->token->cmd->args[i + 1]);
		i++;
	}
	free(root->token->cmd->args[i]);
	root->token->cmd->args[i] = NULL;
}

void	handle_expand(t_ast *root, t_mini *last)
{
	int	i;

	i = 0;
	while (root->token->cmd->args[i])
	{
		root->token->cmd->args[i] = ft_verif_arg(root->token->cmd->args,
				&last->env, root->token->cmd, i);
		i++;
	}
	root->token->cmd->args = ft_trim_quote_args(root->token->cmd->args);
	if (root->token->cmd->args && root->token->cmd->args[0])
	{
		if (root->token->cmd->cmd)
			free(root->token->cmd->cmd);
		root->token->cmd->cmd = ft_strdup(root->token->cmd->args[0]);
	}
	if (root->token->cmd->args && !*root->token->cmd->args[0]
		&& root->token->cmd->args[1])
		ft_handle_empty_first_arg(root);
}
int	ft_cmdsize(t_cmd *cmd)
{
	int	i;
	t_cmd *tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void close_fd(int *fd, int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
}

int first_child(t_cmd *cmd)
{
	close(cmd->exec->pipe_fd[0]);
	dup2(cmd->exec->pipe_fd[1], STDOUT_FILENO);
	close(cmd->exec->pipe_fd[1]);
	printf("REDIR IN = %d\n", cmd->exec->redir_in);
	if (cmd->exec->redir_in != -1)
	{
		dup2(cmd->exec->redir_in, STDIN_FILENO);
		close(cmd->exec->redir_in);
	}
	return (0);
}

int last_child(t_cmd *cmd)
{
	close_fd(cmd->exec->pipe_fd, -1);
	if (cmd->exec->prev_fd != STDIN_FILENO && cmd->exec->prev_fd != -1)
	{
		dup2(cmd->exec->prev_fd, STDIN_FILENO);
		close(cmd->exec->prev_fd);
	}
	printf("REDIR OUT = %d\n", cmd->exec->redir_out);
	if (cmd->exec->redir_out != STDOUT_FILENO && cmd->exec->redir_out != -1)
	{
		dup2(cmd->exec->redir_out, STDOUT_FILENO);
		close(cmd->exec->redir_out);
	}
	return (0);
}

int middle_child(t_cmd *cmd)
{
	close(cmd->exec->pipe_fd[0]);
	dup2(cmd->exec->prev_fd, STDIN_FILENO);
	close(cmd->exec->prev_fd);
	dup2(cmd->exec->pipe_fd[1], STDOUT_FILENO);
	close(cmd->exec->pipe_fd[1]);
	return (0);
}

void	process_child(t_cmd *tmp_cmd, int i, int len_cmd)
{
	// printf("i = %d\n", i);
	// printf("len_cmd = %d\n", len_cmd);
	if (i == 0)
	{
		// printf("FIRST CHILD tmp_cmd->cmd = %s args[1] = %s\n", tmp_cmd->cmd, tmp_cmd->args[1]);
		first_child(tmp_cmd);
	}
	else if (i == len_cmd - 1)
	{
		// printf("LAST CHILD tmp_cmd->cmd = %s args[1] = %s\n", tmp_cmd->cmd, tmp_cmd->args[1]);
		last_child(tmp_cmd);
	}
	else
	{
		// printf("OTHER tmp_cmd->cmd = %s args[1] = %s\n", tmp_cmd->cmd, tmp_cmd->args[1]);
		middle_child(tmp_cmd);
	}
}
int	ft_exec_cmd(t_ast *root, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini	*last;
	t_env	*e_status;
	t_exec	*exec;
	t_cmd	*cmd;
	t_cmd	*tmp_cmd;
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
	tmp_cmd = cmd;
	// gestion des quotes et expand
	if (root->token->type == T_CMD && root->token->cmd)
	{
		if ((root->token->cmd->cmd && root->token->cmd->args)
			|| (!root->token->cmd->cmd && *root->token->cmd->args))
			handle_expand(root, last);
		// premier appel de la fonction pour verifier les file
		if (root->token->cmd->redir)
			handle_redir(root, mini, e_status);
		ft_set_var_underscore(root->token->cmd->args, &last->env, envp);
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
							if (root->token->cmd->redir)
								handle_redir_dup(exec, cmd);
							reset_fd(exec);
							ft_free_tab(envp);
							envp = NULL;
							process_child(cmd, i, len_cmd);
							exec_command(cmd, granny, mini, prompt);
							exit(EXIT_SUCCESS);
						}
						else
						{
							// printf("exec->pipe_fd[0] = %d\n", exec->pipe_fd[0]);
							// printf("exec->pipe_fd[1] = %d\n", exec->pipe_fd[1]);
							// printf("exec->prev_fd = %d\n", exec->prev_fd);
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
					}
					reset_fd(exec);
					cmd = cmd->next;
				}
				if (envp)
					ft_free_tab(envp);
				envp = NULL;
				cmd = tmp_cmd;
				// while (cmd)
				// {
				// 	printf("FOR WAIT tmp_cmd->cmd = %s\n", cmd->cmd);
				// 	waitpid(cmd->exec->pid, &cmd->exec->status, 0);
				// 	cmd = cmd->next;
				// }
				// cmd = tmp_cmd;
				// while (waitpid(-1, &exec->status, 0) > 0)
				// 	;
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
						// execution de la commande
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
