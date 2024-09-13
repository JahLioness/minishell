/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 15:13:36 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/13 17:12:41 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_cat_open_file(t_exec *exec, t_cmd *cmd, int i)
{
	exec->redir_in = open(cmd->args[i], O_RDONLY);
	if (exec->redir_in < 0)
	{
		msg_error("minishell: cat : ", cmd->args[i], strerror(errno));
		exec->error_ex = 1;
	}
}

void	cat_wt_symbole(t_cmd *cmd, t_exec *exec)
{
	int	i;

	i = 0;
	while (cmd->args[++i])
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
			ft_cat_open_file(exec, cmd, i);
			reset_fd(exec);
		}
	}
}

static void	ft_redir_in(t_redir *tmp_redir, t_exec *exec, t_cmd *cmd)
{
	exec->redir_in = open(tmp_redir->file, O_RDONLY);
	if (exec->redir_in < 0)
	{
		msg_error("minishell: ", tmp_redir->file, strerror(errno));
		exec->error_ex = 1;
	}
	else
	{
		if (exec->redir_in != -1 && exec->redir_in != STDIN_FILENO)
			close(exec->redir_in);
		if (exec->redir_out == -1 && !tmp_redir->prev && !cmd->next)
			exec->redir_out = STDOUT_FILENO;
		else if (tmp_redir->prev && (tmp_redir->prev->type == REDIR_OUTPUT
				|| tmp_redir->prev->type == REDIR_APPEND))
		{
			if (tmp_redir->type == REDIR_OUTPUT)
				exec->redir_out = open(tmp_redir->file,
						O_RDWR | O_CREAT | O_TRUNC, 0644);
			else if (tmp_redir->type == REDIR_APPEND)
				exec->redir_out = open(tmp_redir->file,
						O_RDWR | O_CREAT | O_APPEND, 0644);
		}
	}
}

void	builtin_w_redir(t_redir *tmp_redir, t_exec *exec, t_cmd *cmd)
{
	while (tmp_redir)
	{
		if (tmp_redir->type == REDIR_OUTPUT)
			set_redir_output(tmp_redir, exec);
		else if (tmp_redir->type == REDIR_APPEND)
			set_redir_append(tmp_redir, exec);
		else if (tmp_redir->type == REDIR_INPUT)
			ft_redir_in(tmp_redir, exec, cmd);
		tmp_redir = tmp_redir->next;
	}
}
