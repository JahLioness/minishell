/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:52:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/26 13:39:27 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_newtoken(char *cell, t_token_type type, int *i)
{
	t_token	*new;

	new = ft_token_init();
	if (!new)
		return (NULL);
	new->type = type;
	if (type == T_AND || type == T_AND_E || type == T_OR
		|| type == O_BRACKET || type == C_BRACKET)
	{
		if (type == T_AND || type == T_OR)
			*i += 2;
		else
			(*i)++;
		return (new);
	}
	else if (type == T_CMD)
		ft_init_token_cmd(new, cell, i);
	return (new);
}

static void	ft_switch(char *line, int *i, t_mini *mini)
{
	if (line[*i] && line[*i] == '(')
		ft_tokenadd_back(&mini->tokens, ft_newtoken(line, O_BRACKET,
				i));
	else if (line[*i] && line[*i] == ')')
		ft_tokenadd_back(&mini->tokens, ft_newtoken(line, C_BRACKET,
				i));
	else if (line[*i] && line[*i] == '|' && line[*i + 1] == '|')
		ft_tokenadd_back(&mini->tokens, ft_newtoken(line, T_OR, i));
	else if (line[*i] && line[*i] == '&' && line[*i + 1] == '&')
		ft_tokenadd_back(&mini->tokens, ft_newtoken(line, T_AND,
				i));
	else if (line[*i] && line[*i] == '&' && line[*i + 1] != '&')
		ft_tokenadd_back(&mini->tokens, ft_newtoken(line, T_AND_E,
				i));
	else
		ft_tokenadd_back(&mini->tokens, ft_newtoken(line, T_CMD,
				i));
}

void	ft_init_token(t_mini *mini, char *line)
{
	int	i;

	i = 0;
	if (!line)
		return ;
	while (line[i])
	{
		while (line[i] && ft_is_whitespaces(line[i]))
			i++;
		ft_switch(line, &i, mini);
	}
}
