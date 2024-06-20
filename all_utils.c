/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:04:18 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/20 10:21:57 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_quote_pair(char *cell, int *i, char *quote)
{
	int	j;

	*quote = cell[*i];
	j = *i;
	(*i)++;
	if (cell[*i] == *quote)
	{
		*quote = 0;
		(*i)++;
	}
	return (j);
}

char	*ft_get_content_util(char *cell, int i, char *ret)
{
	char	*tmp;

	tmp = ft_substr(cell, i, 1);
	if (!tmp)
		return (ret);
	ret = ft_strjoin_free(ret, tmp);
	free(tmp);
	return (ret);
}

static char	*ft_join_content(char *cell, char *ret, int i)
{
	char	*tmp;

	tmp = ft_substr(cell, i, 1);
	if (!tmp)
		return (ret);
	ret = ft_strjoin_free(ret, tmp);
	free(tmp);
	return (ret);
}

char	*ft_get_content(char *cell, char *quote, int *i, int j)
{
	char	*ret;

	ret = ft_calloc(ft_strlen(cell) + 1, sizeof(char));
	if (!ret)
		return (NULL);
	if (*quote)
	{
		ret = ft_get_content_util(cell, j, ret);
		*i = j + 1;
	}
	while (cell[*i] && ((cell[*i] != '"' && cell[*i] != '\'') || (*quote
				&& cell[*i] != *quote)))
	{
		ret = ft_join_content(cell, ret, *i);
		(*i)++;
	}
	if (*quote)
	{
		ret = ft_get_content_util(cell, *i, ret);
		*quote = 0;
		(*i)++;
	}
	return (ret);
}

char	*ft_trim_empty_quotes(char *cell)
{
	int		i;
	int		j;
	char	quote;
	char	*ret;
	char	*tmp;

	i = 0;
	quote = 0;
	j = 0;
	ret = ft_calloc(ft_strlen(cell) + 1, sizeof(char));
	while (cell[i])
	{
		if (cell[i] == '"' || cell[i] == '\'')
			j = ft_check_quote_pair(cell, &i, &quote);
		if (cell[i])
		{
			tmp = ft_get_content(cell, &quote, &i, j);
			ret = ft_strjoin_free(ret, tmp);
			free(tmp);
		}
	}
	return (ret);
}
