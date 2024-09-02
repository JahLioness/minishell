/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:09 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/02 12:29:25 by ede-cola         ###   ########.fr       */
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
	t_cmd	*tmp_cmd;

	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		tmp = tmp_cmd->redir;
		while (tmp)
		{
			if (tmp->file_heredoc && access(tmp->file_heredoc, F_OK) == 0)
				unlink(tmp->file_heredoc);
			tmp = tmp->next;
		}
		tmp_cmd = tmp_cmd->next;
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
	(void)last;
	if (cmd->redir)
		ft_handle_redir_file(cmd, last);
	if (cmd->redir && cmd->exec.redir_out != -1 && cmd->exec.redir_out != STDOUT_FILENO)
	{
		dup2(cmd->exec.redir_out, STDOUT_FILENO);
		// close(cmd->exec.redir_out);
	}
	if (cmd->redir && cmd->exec.redir_in != -1 && cmd->exec.redir_in != STDIN_FILENO)
	{
		dup2(cmd->exec.redir_in, STDIN_FILENO);
		// close(cmd->exec.redir_in);
	}
	else if (!cmd->redir && !ft_strcmp(cmd->args[0], "cat") && cmd->args[1])
	{
		cat_wt_symbole(cmd, exec);
		dup2(cmd->exec.redir_in, STDIN_FILENO);
		dup2(cmd->exec.redir_out, STDOUT_FILENO);
	}
}

// void	ft_handle_redir_file(t_cmd *cmd, t_mini *last)
// {
// 	t_redir	*current;
// 	t_exec	*exec;
// 	char	*file;

// 	current = cmd->redir;
// 	exec = &cmd->exec;
// 	if (ft_is_expandable(current->file))
// 		file = handle_expand_heredoc(cmd, last, current->file);
// 	else
// 		file = ft_trim_quote(current->file, 0, 0);
// 	if (cmd->heredoc)
// 		free(current->file);
// 	current->file = file;
// 	if (!ft_strcmp(current->file, "*"))
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(current->file, 2);
// 		ft_putendl_fd(": ambiguous redirect", 2);
// 		exec->error_ex = 1;
// 		return ;
// 	}
// 	set_redir(current, exec, cmd);
// }

void	ft_handle_redir_file(t_cmd *cmd, t_mini *last)
{
	t_redir	*current;
	t_exec	*exec;
	char	*file;

	current = cmd->redir;
	exec = &cmd->exec;
	while (current)
	{
		if (ft_is_expandable(current->file) && current->type != REDIR_HEREDOC)
			file = handle_expand_heredoc(cmd, last, current->file);
		else
		{
			file = ft_trim_quote(current->file, 0, 0);
			free(current->file);
		}
		current->file = file;
		if (!ft_strcmp(current->file, "*"))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(current->file, 2);
			ft_putendl_fd(": ambiguous redirect", 2);
			exec->error_ex = 1;
			return ;
		}
		set_redir(current, exec, cmd);
		current = current->next;
	}
}
