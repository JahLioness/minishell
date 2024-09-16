/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:02:14 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/16 16:55:24 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_single_cmd(t_exec_utils *e_utils, t_cmd *cmd, t_exec *exec,
		t_mini *last)
{
	ft_get_signal_cmd();
	exec->pid = fork();
	if (exec->pid < 0)
		return (ft_putendl_fd("minishell: fork failed", 2), 1);
	if (exec->pid == 0)
	{
		if (cmd->redir)
			handle_redir_dup(exec, cmd, last);
		reset_fd(exec);
		ft_free_envp(e_utils);
		exec_command(cmd, e_utils);
		exit(EXIT_SUCCESS);
	}
	else
	{
		ft_free_envp(e_utils);
		reset_fd(exec);
		waitpid(exec->pid, &exec->status, 0);
		if (cmd->redir)
			unlink_files(cmd);
		if (WIFEXITED(exec->status))
			return (handle_sigint(exec, last));
		return (0);
	}
}

void	ft_execution(t_ast *root, t_exec_utils *e_utils, t_cmd *cmd,
		t_exec *exec)
{
	t_mini	*last;
	int		len_cmd;

	last = ft_minilast(*e_utils->mini);
	len_cmd = ft_cmdsize(cmd);
	if (len_cmd > 1)
		exec->status = ft_exec_lst_cmd(root, e_utils);
	else if (len_cmd == 1)
	{
		if ((cmd->cmd && cmd->args) || (!cmd->cmd && *cmd->args))
			handle_expand(cmd, last);
		if (cmd->redir)
			handle_redir(cmd, e_utils->mini);
		ft_set_var_underscore(cmd->args, &last->env, e_utils->envp);
		if (exec->error_ex)
			unlink_files(cmd);
		if (!exec->error_ex)
		{
			reset_fd(exec);
			ft_exec_builtins(cmd, e_utils);
			if (!ft_is_builtin(cmd->cmd) && ft_strcmp(cmd->cmd, "exit"))
				exec->status = ft_exec_single_cmd(e_utils, cmd, exec, last);
		}
	}
}

int	ft_exec_cmd(t_ast *root, t_exec_utils *e_utils)
{
	t_mini	*last;
	t_exec	*exec;
	t_cmd	*cmd;

	cmd = root->token->cmd;
	exec = &cmd->exec;
	e_utils->current = root;
	last = ft_minilast(*e_utils->mini);
	e_utils->envp = ft_get_envp(&last->env);
	if (root->token->type == T_CMD && root->token->cmd)
		ft_execution(root, e_utils, cmd, exec);
	ft_free_envp(e_utils);
	if (g_sig == SIGQUIT)
		handle_sigquit(exec, last);
	return (exec->status);
}

void	ft_check_builtin(t_cmd *cmd, t_exec_utils *e_utils, t_mini *last)
{
	int	status;

	if (ft_is_builtin(cmd->cmd) && ft_strcmp(cmd->cmd, "exit"))
	{
		handle_builtin(cmd, last, cmd->redir, &cmd->exec);
		status = cmd->exec.status;
		close_fd(cmd->exec.pipe_fd, -1);
		reset_fd(&cmd->exec);
		ft_exec_cmd_error(e_utils, e_utils->envp);
		exit(status);
	}
	else if (!ft_strcmp(cmd->cmd, "exit"))
	{
		ft_free_envp(e_utils);
		handle_exit(e_utils, cmd);
		exit(EXIT_FAILURE);
	}
}

void	ft_expand_redir_gestion(t_cmd *cmd, t_exec_utils *e_utils, t_mini *last)
{
	if ((cmd->cmd && cmd->args) || (!cmd->cmd && *cmd->args))
		handle_expand(cmd, last);
	if (cmd->redir)
		handle_redir(cmd, e_utils->mini);
}
