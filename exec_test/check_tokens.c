/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:37 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/22 12:36:10 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_operator(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == T_AND || tmp->type == T_OR)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	ft_is_pipe(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == T_PIPE)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int	ft_is_bracket(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (tmp->type == O_BRACKET || tmp->type == C_BRACKET)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
