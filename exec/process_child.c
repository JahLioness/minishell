/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_child.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:36:42 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/17 15:24:21 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_fd(int *fd, int prev_fd)
{
	if (prev_fd != -1 && prev_fd != STDIN_FILENO)
		close(prev_fd);
	if (fd[0] != -1 && fd[0] != STDIN_FILENO)
		close(fd[0]);
	if (fd[1] != -1 && fd[1] != STDOUT_FILENO)
		close(fd[1]);
}

int	first_child(t_cmd *cmd)
{
	if ((!cmd->next->redir || (cmd->next->redir->type != REDIR_HEREDOC
				&& cmd->next->redir->type != REDIR_INPUT))
		&& cmd->exec.pipe_fd[0] != -1 && cmd->exec.pipe_fd[0] != STDIN_FILENO)
		close(cmd->exec.pipe_fd[0]);
	if (cmd->exec.pipe_fd[1] != -1 && cmd->exec.pipe_fd[1] != STDOUT_FILENO)
	{
		dup2(cmd->exec.pipe_fd[1], STDOUT_FILENO);
		close(cmd->exec.pipe_fd[1]);
	}
	if (cmd->exec.redir_in != -1 && cmd->exec.redir_in != STDIN_FILENO)
	{
		dup2(cmd->exec.redir_in, STDIN_FILENO);
		close(cmd->exec.redir_in);
	}
	if (cmd->exec.redir_out != STDOUT_FILENO && cmd->exec.redir_out != -1)
	{
		dup2(cmd->exec.redir_out, STDOUT_FILENO);
		close(cmd->exec.redir_out);
	}
	return (0);
}

int	last_child(t_cmd *cmd)
{
	close_fd(cmd->exec.pipe_fd, -1);
	if (cmd->exec.prev_fd != STDIN_FILENO && cmd->exec.prev_fd != -1)
	{
		dup2(cmd->exec.prev_fd, STDIN_FILENO);
		close(cmd->exec.prev_fd);
	}
	if (cmd->exec.redir_out != STDOUT_FILENO && cmd->exec.redir_out != -1)
	{
		dup2(cmd->exec.redir_out, STDOUT_FILENO);
		close(cmd->exec.redir_out);
	}
	return (0);
}

int	middle_child(t_cmd *cmd)
{
	if (cmd->exec.pipe_fd[0] != -1 && cmd->exec.pipe_fd[0] != STDIN_FILENO)
		close(cmd->exec.pipe_fd[0]);
	if (cmd->exec.prev_fd != STDIN_FILENO && cmd->exec.prev_fd != -1)
	{
		dup2(cmd->exec.prev_fd, STDIN_FILENO);
		close(cmd->exec.prev_fd);
	}
	if (cmd->exec.pipe_fd[1] != -1 && cmd->exec.pipe_fd[1] != STDOUT_FILENO)
	{
		dup2(cmd->exec.pipe_fd[1], STDOUT_FILENO);
		close(cmd->exec.pipe_fd[1]);
	}
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
