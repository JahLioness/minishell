/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:51:09 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/22 13:26:25 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_clear_token(t_token **token)
{
	t_token	*tmp;
	t_cmd	*tmp_cmd;

	while (*token)
	{
		tmp = *token;
		*token = (*token)->next;
		if (tmp->cmd)
		{
			while (tmp->cmd)
			{
				tmp_cmd = tmp->cmd;
				tmp->cmd = tmp->cmd->next;
				free(tmp_cmd->exec);
				if (tmp_cmd->args)
					ft_free_tab(tmp_cmd->args);
				if (tmp_cmd->cmd && !ft_is_builtin(tmp_cmd->cmd)
					&& tmp_cmd->redir)
					ft_clear_redir(tmp_cmd->redir);
				else if (tmp_cmd && tmp_cmd->redir)
					ft_clear_token_redir(tmp_cmd->redir);
				if (tmp_cmd->cmd)
					free(tmp_cmd->cmd);
				free(tmp_cmd);
			}
		}
		free(tmp);
	}
}

t_token	*ft_tokenlast(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return (NULL);
	tmp = token;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_tokenadd_back(t_token **token, t_token *new)
{
	t_token	*last;

	if (!token || !new)
		return ;
	if (!*token)
		*token = new;
	else
	{
		last = ft_tokenlast(*token);
		last->next = new;
		new->prev = last;
	}
}

t_token	*ft_token_init(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = 0;
	token->status = -1;
	token->cmd = NULL;
	token->prev = NULL;
	token->next = NULL;
	return (token);
}

void	ft_token_delone(t_token *token)
{
	if (!token)
		return ;
	ft_token_delone_utils(token);
	if (token->prev && token->next)
	{
		token->prev->next = token->next;
		token->next->prev = token->prev;
	}
	if (!token->prev && token->next)
		token->next->prev = NULL;
	else if (token->prev && !token->next)
		token->prev->next = NULL;
	token->next = NULL;
	token->prev = NULL;
	free(token);
}
