/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:31:02 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/28 19:11:36 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_token_delone_utils(t_token *token)
{
	if (token->cmd)
	{
		ft_free_tab(token->cmd->args);
		free(token->cmd->cmd);
		if (token->cmd->redir)
		{
			if (token->cmd->redir->file)
				free(token->cmd->redir->file);
			free(token->cmd->redir);
		}
		free(token->cmd);
	}
}

void	ft_clear_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		free(tmp->file);
		if (tmp->file_heredoc)
			free(tmp->file_heredoc);
		free(tmp);
	}
}

void	ft_clear_token_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir;
		redir = redir->next;
		if (tmp->file_heredoc)
		{
			if (access(tmp->file_heredoc, F_OK) == 0)
				unlink(tmp->file_heredoc);
		}
		free(tmp->file_heredoc);
		free(tmp->file);
		free(tmp);
	}
}

void	ft_clear_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	while (*cmd)
	{
		tmp = *cmd;
		*cmd = (*cmd)->next;
		if (tmp->args)
			ft_free_tab(tmp->args);
		if (tmp->cmd && tmp->redir && !tmp->redir->file_heredoc)
			ft_clear_redir(tmp->redir);
		else if (tmp && tmp->redir && !tmp->cmd)
			ft_clear_token_redir(tmp->redir);
		if (tmp->cmd)
			free(tmp->cmd);
		free(tmp);
	}
}
