/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:05:12 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/11 16:17:14 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_builtin(t_cmd *cmd, t_mini *last, t_redir *tmp, t_exec *exec)
{
	char	**envp;

	tmp = cmd->redir;
	envp = ft_get_envp(&last->env);
	if (tmp && tmp->type != REDIR_HEREDOC)
		builtin_w_redir(tmp, exec);
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

void	handle_exit(t_ast *root, t_mini **mini, char *prompt)
{
	t_exec	exec;
	t_mini	*last;
	t_env	*e_status;
	char	**envp;

	exec = root->token->cmd->exec;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&(*mini)->env);
	exec.status = ft_exit(root, mini, prompt, envp);
	e_status = ft_get_exit_status(&last->env);
	ft_change_exit_status(e_status, ft_itoa(exec.status));
	ft_free_tab(envp);
	envp = NULL;
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
	// printf("status: %d\n",exec->status);
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
		if (cmd->redir)
			unlink_files(cmd);
		if (cmd->next)
			cmd = cmd->next;
		i++;
	}
	if (WIFEXITED(cmd->exec.status))
	{
		// printf("status: %d\n", WIFEXITED(cmd->exec.status));
		// cmd->exec.status = WIFEXITED(cmd->exec.status);
		set_e_status(WIFEXITED(cmd->exec.status), last);
		return (handle_sigint(&cmd->exec, last));
	}
	// printf("status: %d\n", cmd->exec.status);
	return (cmd->exec.status);
}
