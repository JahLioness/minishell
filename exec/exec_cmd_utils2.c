/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:05:12 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/21 15:09:25 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_builtin(t_ast *root, t_mini *last, t_redir *tmp, t_exec *exec)
{
	char	**envp;

	envp = ft_get_envp(&last->env);
	if (tmp && tmp->type != REDIR_HEREDOC)
		builtin_w_redir(tmp, exec);
	else
		exec->redir_out = STDOUT_FILENO;
	exec->status = ft_exec_builtin(root->token, &last->env, exec->redir_out);
	reset_fd(exec);
	ft_free_tab(envp);
	envp = NULL;
}

void	handle_exit(t_ast *root, t_mini **mini, t_env *e_status, char *prompt)
{
	t_exec	*exec;
	t_mini	*last;
	char	**envp;

	exec = root->token->cmd->exec;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&(*mini)->env);
	exec->status = ft_exit(root, mini, prompt, envp);
	e_status = ft_get_exit_status(&last->env);
	ft_change_exit_status(e_status, ft_itoa(exec->status));
	ft_free_tab(envp);
	envp = NULL;
}

int	handle_sigint(t_exec *exec, t_mini *last, t_env *e_status)
{
	if (WIFEXITED(exec->status))
	{
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
	return (exec->status);
}

int	handle_sigquit(char **envp, t_exec *exec, t_env *e_status)
{
	if (g_sig == SIGQUIT)
	{
		if (envp)
			ft_free_tab(envp);
		reset_fd(exec);
		ft_change_exit_status(e_status, ft_itoa(131));
		g_sig = 0;
		return (131);
	}
	return (exec->status);
}