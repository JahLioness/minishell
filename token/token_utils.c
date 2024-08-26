/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:51:09 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/26 15:38:29 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_clear_token(t_token **token)
{
	t_token	*tmp;

	while (*token)
	{
		tmp = *token;
		*token = (*token)->next;
		if (tmp->cmd)
			ft_clear_cmd(&tmp->cmd);
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
