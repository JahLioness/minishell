/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:58:47 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/17 15:15:14 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_check_cmd_node_hd(t_cmd *cmd, t_mini **mini, char *prompt)
{
	t_cmd	*heredoc;

	while (cmd)
	{
		if (cmd->redir && cmd->redir->type == REDIR_HEREDOC)
		{
			heredoc = get_heredoc_node(cmd);
			handle_heredoc(heredoc, mini, prompt);
		}
		cmd = cmd->next;
	}
}

void	ft_get_heredoc_loop(t_token *token, t_mini **mini, char *prompt)
{
	t_token	*tmp;
	t_cmd	*cmd;
	t_cmd	*heredoc;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == T_CMD)
		{
			cmd = tmp->cmd;
			if (cmd->next)
				ft_check_cmd_node_hd(cmd, mini, prompt);
			else if (cmd->redir && cmd->redir->type == REDIR_HEREDOC)
			{
				heredoc = get_heredoc_node(cmd);
				handle_heredoc(heredoc, mini, prompt);
			}
		}
		tmp = tmp->next;
	}
}
