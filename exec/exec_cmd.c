/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/20 13:26:56 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_builtin(t_token *token, t_env **env, int fd)
{
	t_env	*status;
	int		exit_status;

	exit_status = -1;
	if (!ft_strcmp(token->cmd->cmd, "echo"))
		exit_status = ft_echo(fd, ft_get_args_echo(token->cmd->args, env),
				ft_get_flag_echo(token->cmd->args));
	else if (!ft_strcmp(token->cmd->cmd, "cd"))
		exit_status = ft_cd(token->cmd->args[1], env);
	else if (!ft_strcmp(token->cmd->cmd, "pwd"))
		exit_status = ft_pwd(fd, env);
	else if (!ft_strcmp(token->cmd->cmd, "export"))
		exit_status = ft_exec_export(token, env, fd);
	else if (!ft_strcmp(token->cmd->cmd, "unset"))
		exit_status = ft_exec_unset(token->cmd, env);
	else if (!ft_strcmp(token->cmd->cmd, "env"))
		exit_status = ft_print_env(env, fd);
	status = ft_get_exit_status(env);
	if (status)
		ft_change_exit_status(status, ft_itoa(exit_status));
	else
		ft_envadd_back(env, ft_envnew(ft_strdup("?"),
				ft_itoa(exit_status)));
	return (exit_status);
}

int	ft_exec_cmd_path(t_ast *root, t_mini **mini, char **envp, char *prompt)
{
	char	*cmd_path;
	t_mini	*last;

	last = ft_minilast(*mini);
	if (!ft_strcmp(root->token->cmd->cmd, "$_"))
		cmd_path = ft_tabchr(envp, "_", '=');
	else if (ft_strchr(root->token->cmd->cmd, '/'))
		cmd_path = ft_strdup(root->token->cmd->cmd);
	else
		cmd_path = ft_get_cmd_path_env(root->token->cmd->cmd, envp);
	if (!cmd_path)
	{
		if (execve(root->token->cmd->cmd, root->token->cmd->args, envp) == -1)
			ft_exec_cmd_error(root, mini, envp, prompt);
	}
	else
	{
		if (execve(cmd_path, root->token->cmd->args, envp) == -1)
		{
			free(cmd_path);
			ft_exec_cmd_error(root, mini, envp, prompt);
		}
	}
	return (0);
}

int	ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

int	ft_handle_redir_file(int redir_fd, t_ast *root)
{
	t_token	*current;

	current = root->token;
	while (current && current->type == T_CMD)
	{
		if (current->cmd && current->cmd->redir)
		{
			if (current->cmd->redir->type == REDIR_INPUT)
			{
				if (redir_fd != -1)
					close(redir_fd);
				redir_fd = open(current->cmd->redir->file, O_RDONLY);
				if (redir_fd < 0)
					return (ft_putendl_fd("minishell: open failed", 2), 1);
			}
			else if (current->cmd->redir->type == REDIR_OUTPUT)
			{
				if (redir_fd != -1)
					close(redir_fd);
				redir_fd = open(current->cmd->redir->file,
						O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (redir_fd < 0)
					return (ft_putendl_fd("minishell: open failed", 2), 1);
			}
		}
		current = current->next;
	}
	return (redir_fd);
}

int	ft_exec_cmd(t_ast *root, t_mini **mini, char *prompt)
{
	t_mini	*last;
	pid_t	pid;
	int		status;
	int		fd;
	int		redir_fd;
	t_env	*e_status;
	char	**envp;

	fd = -1;
	redir_fd = -1;
	status = -1;
	redir_fd = ft_handle_redir_file(redir_fd, root);
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	if (root->token->type == T_CMD && root->token->cmd)
	{
		int i = 0;
		if (root->token->cmd->args)
		{
			while (root->token->cmd->args[i])
			{
				root->token->cmd->args[i] = ft_verif_arg(root->token->cmd->args, &last->env, root->token->cmd, i);
				i++;
			}
			root->token->cmd->args = ft_trim_quote_args(root->token->cmd->args);
			free(root->token->cmd->cmd);
			root->token->cmd->cmd = ft_strdup(root->token->cmd->args[0]);
		}
		ft_set_var_underscore(root->token->cmd->args, &last->env, envp);
		if (redir_fd == -1)
			redir_fd = STDOUT_FILENO;
		if (ft_is_builtin(root->token->cmd->cmd))
		{
			status = ft_exec_builtin(root->token, &last->env, redir_fd);
			if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
				close(redir_fd);
			ft_free_tab(envp);
			envp = NULL;
		}
		else
		{
			pid = fork();
			if (pid < 0)
				return (ft_putendl_fd("minishell: fork failed", 2), 1);
			if (pid == 0)
			{
				if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
				{
					if (root->token->cmd->redir->type == REDIR_INPUT)
						dup2(redir_fd, STDIN_FILENO);
					else if (root->token->cmd->redir->type == REDIR_OUTPUT)
						dup2(redir_fd, STDOUT_FILENO);
					close(redir_fd);
				}
				else if (redir_fd == -1 && root->token->cmd->args[1])
				{
					fd = open(root->token->cmd->args[1], O_RDONLY);
					if (fd < 0)
					{
						perror("open");
						exit(EXIT_FAILURE);
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
				}
				ft_exec_cmd_path(root, mini, envp, prompt);
				exit(EXIT_SUCCESS);
			}
			else
			{
				if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
					close(redir_fd);
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
				{
					e_status = ft_get_exit_status(&last->env);
					if (e_status)
						ft_change_exit_status(e_status,
							ft_itoa(WEXITSTATUS(status)));
					else
						ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"),
								ft_itoa(WEXITSTATUS(status))));
					printf("status: %d\n", WEXITSTATUS(status));
					ft_free_tab(envp);
					return (WEXITSTATUS(status));
				}
			}
		}
	}
	if (envp)
		ft_free_tab(envp);
	return (status);
}
