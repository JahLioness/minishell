/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:09 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/26 19:00:27 by ede-cola         ###   ########.fr       */
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

void	unlink_files(t_redir *redir)
{
	t_redir	*tmp;

	tmp = redir;
	while (tmp)
	{
		if (tmp->type == REDIR_HEREDOC && access(tmp->file_heredoc, F_OK) == 0)
			unlink(tmp->file_heredoc);
		tmp = tmp->next;
	}
}

void	handle_redir(t_cmd *cmd, t_mini **mini, t_env *e_status)
{
	t_exec	*exec;
	t_mini	*last;

	exec = cmd->exec;
	last = ft_minilast(*mini);
	if (cmd->redir)
	{
		ft_handle_redir_file(cmd);
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

void	handle_redir_dup(t_exec *exec, t_cmd *cmd)
{
	if (cmd->redir)
		ft_handle_redir_file(cmd);
	if (cmd->redir && ((exec->redir_in != -1 && exec->redir_in != STDOUT_FILENO)
			|| (exec->redir_out != -1 && exec->redir_out != STDOUT_FILENO)))
	{
		dup2(exec->redir_in, STDIN_FILENO);
		dup2(exec->redir_out, STDOUT_FILENO);
	}
	else if (!cmd->redir && !ft_strcmp(cmd->args[0], "cat") && cmd->args[1])
	{
		cat_wt_symbole(cmd, exec);
		dup2(exec->redir_in, STDIN_FILENO);
		dup2(exec->redir_out, STDOUT_FILENO);
	}
}

void	ft_handle_redir_file(t_cmd *cmd)
{
	t_redir	*current;
	t_exec	*exec;
	char	*file;

	current = cmd->redir;
	exec = cmd->exec;
	file = ft_trim_quote(current->file, 0, 0);
	free(current->file);
	current->file = file;
	if (!ft_strcmp(current->file, "*"))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->file, 2);
		ft_putendl_fd(": ambiguous redirect", 2);
		exec->error_ex = 1;
		return ;
	}
	if (cmd->cmd)
		set_redir(current, exec);
}
