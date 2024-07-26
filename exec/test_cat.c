/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_cat.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:35:08 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/25 20:31:11 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cat_wt_symbole(t_cmd *cmd, t_exec *exec)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (access(cmd->args[i], F_OK) == -1)
		{
			exec->error_ex = 1;
			msg_error("minishell: cat : ", cmd->args[i],
					"no such file or directory");
		}
		else if (access(cmd->args[i], R_OK) == -1)
		{
			exec->error_ex = 1;
			msg_error("minishell: cat", cmd->args[i], "Permission denied");
		}
		else
		{
			exec->redir_in = open(cmd->args[i], O_RDONLY);
			if (exec->redir_in < 0)
			{
				msg_error("minishell: cat : ", cmd->args[i], strerror(errno));
				exec->error_ex = 1;
			}
			reset_fd(exec);
		}
		i++;
	}
}

void	builtin_w_redir(t_redir *tmp_redir, t_exec *exec)
{
	while (tmp_redir)
	{
		// if (exec->redir_in != -1)
		// 	close(exec->redir_in);
		// if (exec->redir_out != -1)
		// 	close(exec->redir_out);
		reset_fd(exec);
		if (tmp_redir->type == REDIR_OUTPUT)
			exec->redir_out = open(tmp_redir->file, O_RDWR | O_CREAT | O_TRUNC,
					0644);
		else if (tmp_redir->type == REDIR_APPEND)
			exec->redir_out = open(tmp_redir->file, O_RDWR | O_CREAT | O_APPEND,
					0644);
		else if (tmp_redir->type == REDIR_INPUT)
		{
			exec->redir_in = open(tmp_redir->file, O_RDONLY);
			if (exec->redir_in < 0)
			{
				msg_error("minishell: ", tmp_redir->file, strerror(errno));
				exec->error_ex = 1;
			}
			else
			{
				close(exec->redir_in);
				exec->redir_out = STDOUT_FILENO;
			}
		}
		if (exec->redir_out < 0)
		{
			msg_error("minishell: ", tmp_redir->file, strerror(errno));
			exec->error_ex = 1;
		}
		tmp_redir = tmp_redir->next;
	}
}

// void	check_acces(t_cmd *cmd, t_exec *exec)
// {
// 	if (access(cmd->cmd, F_OK) == -1 && errno == ENOENT)
// 		msg_error("minishell: ", cmd->cmd, "no such file or directory");
// 	else if (access(cmd->cmd, R_OK) == -1 && errno == EACCES)
// 		msg_error("minishell: ", cmd->cmd, "permission denied");
// 	else if (access(cmd->cmd, X_OK) == -1 && errno == EACCES)
// 		msg_error("minishell: ", cmd->cmd, "is a directory");
// 	exec->error_ex = 1;
// }
