/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:03 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/25 17:22:59 by ede-cola         ###   ########.fr       */
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
		if (exec->redir_fd != -1)
			close(exec->redir_fd);
		if (current->type == REDIR_INPUT)
		{
			exec->redir_in = open(current->file, O_RDONLY);
			if (exec->redir_in < 0)
			{
				exec->error_ex = 1;
				return (ft_putstr_fd("minishell: ", 2),
						ft_putstr_fd(current->file, 2),
						ft_putendl_fd(": No such file or directory", 2));
			}
		}
		else if (current->type == REDIR_OUTPUT)
		{
			exec->redir_out = open(current->file, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		}
		else if (current->type == REDIR_APPEND)
		{
			exec->redir_out = open(current->file, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			if (exec->redir_fd != -1)
				close(exec->redir_fd);
			exec->redir_fd = open(current->file_heredoc, O_RDONLY);
			if (exec->redir_fd < 0)
			{
				exec->error_ex = 1;
				return (ft_putstr_fd("minishell: ", 2),
					ft_putstr_fd(current->file, 2),
					ft_putendl_fd(": No such file or directory", 2));
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
