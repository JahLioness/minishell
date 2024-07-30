/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:03 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/25 20:47:46 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	while (current)
	{
		if (current->type == REDIR_INPUT)
		{
			if (exec->redir_in != -1)
				close(exec->redir_in);
			exec->redir_in = open(current->file, O_RDONLY);
			if (exec->redir_in < 0)
			{
				exec->error_ex = 1;
				return (msg_error("minishell: ", current->file,
						"No such file or directory"));
			}
		}
		else if (current->type == REDIR_OUTPUT)
		{
			if (exec->redir_out != -1)
				close(exec->redir_out);
			exec->redir_out = open(current->file, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		}
		else if (current->type == REDIR_APPEND)
		{
			if (exec->redir_out != -1)
				close(exec->redir_out);
			exec->redir_out = open(current->file, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			if (exec->redir_in != -1)
				close(exec->redir_in);
			exec->redir_in = open(current->file_heredoc, O_RDONLY);
			if (exec->redir_in < 0)
			{
				exec->error_ex = 1;
				return (msg_error("minishell: ", current->file_heredoc,
						"No such file or directory"));
			}
		}
		if (access(current->file, R_OK | W_OK) == -1)
		{
			exec->error_ex = 1;
			return (msg_error("minishell: ", current->file, "Permission denied"));
		}
		current = current->next;
	}
}
