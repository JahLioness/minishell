/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:36:42 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/24 19:34:53 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_fd(int *fd, int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
}

int	first_child(t_cmd *cmd)
{
	close(cmd->exec->pipe_fd[0]);
	dup2(cmd->exec->pipe_fd[1], STDOUT_FILENO);
	close(cmd->exec->pipe_fd[1]);
	if (cmd->exec->redir_in != -1)
	{
		dup2(cmd->exec->redir_in, STDIN_FILENO);
		close(cmd->exec->redir_in);
	}
	return (0);
}

int	last_child(t_cmd *cmd)
{
	close_fd(cmd->exec->pipe_fd, -1);
	if (cmd->exec->prev_fd != STDIN_FILENO && cmd->exec->prev_fd != -1)
	{
		dup2(cmd->exec->prev_fd, STDIN_FILENO);
		close(cmd->exec->prev_fd);
	}
	if (cmd->exec->redir_out != STDOUT_FILENO && cmd->exec->redir_out != -1)
	{
		dup2(cmd->exec->redir_out, STDOUT_FILENO);
		close(cmd->exec->redir_out);
	}
	return (0);
}

int	middle_child(t_cmd *cmd)
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
	if (i == 0)
		first_child(tmp_cmd);
	else if (i == len_cmd - 1)
		last_child(tmp_cmd);
	else
		middle_child(tmp_cmd);
}
