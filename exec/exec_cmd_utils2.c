/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:05:12 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/18 11:41:18 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_builtin(t_cmd *cmd, t_mini *last, t_redir *tmp, t_exec *exec)
{
	char	**envp;

	tmp = cmd->redir;
	envp = ft_get_envp(&last->env);
	if (tmp && tmp->type != REDIR_HEREDOC)
		builtin_w_redir(tmp, exec, cmd);
	else
	{
		if (cmd->next)
			exec->redir_out = exec->pipe_fd[1];
		else
			exec->redir_out = STDOUT_FILENO;
	}
	exec->status = ft_exec_builtin(cmd, &last->env, exec->redir_out);
	reset_fd(exec);
	ft_free_tab(envp);
	envp = NULL;
}

void	handle_exit(t_exec_utils *e_utils, t_cmd *cmd)
{
	t_exec	exec;
	t_mini	*last;
	t_env	*e_status;

	exec = cmd->exec;
	last = ft_minilast(*e_utils->mini);
	exec.status = ft_exit(e_utils, cmd);
	e_status = ft_get_exit_status(&last->env);
	ft_change_exit_status(e_status, ft_itoa(exec.status));
	ft_free_tab(e_utils->envp);
	e_utils->envp = NULL;
}

int	handle_sigint(t_exec *exec, t_mini *last)
{
	t_env	*e_status;

	e_status = ft_get_exit_status(&last->env);
	if (g_sig == SIGINT)
	{
		kill(exec->pid, SIGKILL);
		ft_change_exit_status(e_status, ft_itoa(130));
		g_sig = 0;
		return (130);
	}
	return (set_e_status(exec->status, last));
}

int	handle_sigquit(t_exec *exec, t_mini *last)
{
	t_env	*e_status;

	e_status = ft_get_exit_status(&last->env);
	reset_fd(exec);
	ft_change_exit_status(e_status, ft_itoa(131));
	g_sig = 0;
	return (131);
}

int	ft_waitpid(t_cmd *cmd, t_mini *last, int len_cmd)
{
	int	i;

	i = 0;
	while (i < len_cmd)
	{
		close_fd(cmd->exec.pipe_fd, cmd->exec.prev_fd);
		waitpid(cmd->exec.pid, &cmd->exec.status, 0);
		if (cmd->next)
			cmd = cmd->next;
		i++;
	}
	if (WIFEXITED(cmd->exec.status))
	{
		set_e_status(WIFEXITED(cmd->exec.status), last);
		return (handle_sigint(&cmd->exec, last));
	}
	return (cmd->exec.status);
}
