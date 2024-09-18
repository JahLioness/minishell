/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:09 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/18 11:46:07 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_fd(t_exec *exec)
{
	if (exec->redir_out != -1 && exec->redir_out != STDOUT_FILENO)
	{
		close(exec->redir_out);
		exec->redir_out = -1;
	}
	if (exec->redir_in != -1 && exec->redir_in != STDIN_FILENO)
	{
		close(exec->redir_in);
		exec->redir_in = -1;
	}
}

void	unlink_files(t_cmd *cmd)
{
	t_redir	*tmp;

	tmp = cmd->redir;
	while (tmp)
	{
		if (cmd->redir && cmd->redir->file_heredoc
			&& access(cmd->redir->file_heredoc, F_OK) == 0)
			unlink(cmd->redir->file_heredoc);
		tmp = tmp->next;
	}
}

void	handle_redir(t_cmd *cmd, t_mini **mini)
{
	t_exec	*exec;
	t_mini	*last;
	t_env	*e_status;

	exec = &cmd->exec;
	last = ft_minilast(*mini);
	if (cmd->redir)
	{
		ft_handle_redir_file(cmd, last);
		reset_fd(exec);
	}
	else if (!cmd->redir && cmd->args && !ft_strcmp(cmd->args[0], "cat")
		&& cmd->args[1])
		cat_wt_symbole(cmd, exec);
	if (exec->error_ex == 1 || (cmd->redir && cmd->redir->type != REDIR_HEREDOC
			&& !cmd->cmd))
	{
		reset_fd(exec);
		e_status = ft_get_exit_status(&last->env);
		ft_change_exit_status(e_status, ft_itoa(1));
	}
}

void	handle_redir_dup(t_exec *exec, t_cmd *cmd, t_mini *last)
{
	if (cmd->redir)
		ft_handle_redir_file(cmd, last);
	if (cmd->redir && cmd->exec.redir_out != -1
		&& cmd->exec.redir_out != STDOUT_FILENO)
		dup2(cmd->exec.redir_out, STDOUT_FILENO);
	if (cmd->redir && cmd->exec.redir_in != -1
		&& cmd->exec.redir_in != STDIN_FILENO)
		dup2(cmd->exec.redir_in, STDIN_FILENO);
	else if (!cmd->redir && !ft_strcmp(cmd->args[0], "cat") && cmd->args[1])
	{
		cat_wt_symbole(cmd, exec);
		dup2(cmd->exec.redir_in, STDIN_FILENO);
		dup2(cmd->exec.redir_out, STDOUT_FILENO);
	}
}

void	ft_handle_redir_file(t_cmd *cmd, t_mini *last)
{
	t_redir	*current;
	t_exec	*exec;

	current = cmd->redir;
	exec = &cmd->exec;
	while (current)
	{
		ft_handle_redir_file_loop(current, exec, cmd, last);
		if (cmd->exec.error_ex == 1)
		{
			set_e_status(1, last);
			break ;
		}
		current = current->next;
	}
}
