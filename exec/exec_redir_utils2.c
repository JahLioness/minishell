/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 16:13:57 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/06 16:16:36 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_handle_redir_file_loop(t_redir *redir, t_exec *exec, t_cmd *cmd,
		t_mini *last)
{
	char	*file;

	if (ft_is_expandable(redir->file) && redir->type != REDIR_HEREDOC)
		file = handle_expand_heredoc(cmd, last, redir->file);
	else
	{
		file = ft_trim_quote(redir->file, 0, 0);
		free(redir->file);
	}
	redir->file = file;
	if (!ft_strcmp(redir->file, "*"))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putendl_fd(": ambiguous redirect", 2);
		exec->error_ex = 1;
		return ;
	}
	set_redir(redir, exec, cmd);
}
