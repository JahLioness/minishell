/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:09 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/23 15:56:27 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_fd(int fd)
{
	if (fd != -1 && fd != STDOUT_FILENO)
	{
		close(fd);
		fd = -1;
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

// void	reset_fd(t_cmd *cmd)
// {
// 	while (cmd->redir)
// 	{
// 		if (cmd->exec->redir_fd != -1 && cmd->exec->redir_fd != STDOUT_FILENO)
// 		{
// 			close(cmd->exec->redir_fd);
// 			cmd->exec->redir_fd = -1;
// 		}
// 		// close_fd(cmd->exec->redir_fd);
// 		cmd->redir = cmd->redir->next;
// 	}
// }