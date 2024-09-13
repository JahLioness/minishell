/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:13:29 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/13 17:12:04 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_redir_input(t_redir *current, t_exec *exec)
{
	if (exec->redir_in != -1 && exec->redir_in != STDIN_FILENO)
		close(exec->redir_in);
	exec->redir_in = open(current->file, O_RDONLY);
	if (exec->redir_in == -1)
	{
		exec->error_ex = 1;
		exec->status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
}

void	set_redir_output(t_redir *current, t_exec *exec)
{
	if (exec->redir_out != -1 && exec->redir_out != STDOUT_FILENO)
		close(exec->redir_out);
	exec->redir_out = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (exec->redir_out == -1)
	{
		exec->error_ex = 1;
		exec->status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
}

void	set_redir_append(t_redir *current, t_exec *exec)
{
	if (exec->redir_out != -1 && exec->redir_out != STDOUT_FILENO)
		close(exec->redir_out);
	exec->redir_out = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (exec->redir_out == -1)
	{
		exec->error_ex = 1;
		exec->status = 1;
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->file, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
}

static void	set_redir_heredoc(t_redir *current, t_exec *exec)
{
	if (exec->redir_in != -1)
		close(exec->redir_in);
	exec->redir_in = open(current->file_heredoc, O_RDONLY);
	if (exec->redir_in < 0)
	{
		exec->error_ex = 1;
		if (access(current->file_heredoc, F_OK) == -1)
			return (msg_error("minishell: ", current->file_heredoc,
					"No such file or directory"));
		else if (access(current->file_heredoc, R_OK | W_OK | X_OK) == -1)
			return (msg_error("minishell: ", current->file_heredoc,
					"Permission denied"));
	}
}

void	set_redir(t_redir *current, t_exec *exec, t_cmd *cmd)
{
	if (current->type == REDIR_INPUT)
		set_redir_input(current, exec);
	else if (current->type == REDIR_OUTPUT)
		set_redir_output(current, exec);
	else if (current->type == REDIR_APPEND)
		set_redir_append(current, exec);
	else if (current->type == REDIR_HEREDOC && cmd->cmd)
		set_redir_heredoc(current, exec);
}
