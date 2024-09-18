/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:12:04 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 13:34:11 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_whitespace(char *str, int i)
{
	while (str[i] && ft_is_whitespaces(str[i]))
		i++;
	return (i);
}

int	ft_is_not_stop(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == '&' || c == '\0' || c == '('
		|| c == ')')
		return (0);
	return (1);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	ret = ft_calloc((ft_strlen(s1) + ft_strlen(s2)) + 1, sizeof(char));
	if (!ret)
		return (NULL);
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		ret[i] = s2[j];
		i++;
		j++;
	}
	ret[i] = '\0';
	free(s1);
	return (ret);
}

char	*ft_trim_quote(char *line, int i, int j)
{
	char	*ret;
	char	quote;

	quote = 0;
	ret = ft_calloc((ft_strlen(line) + 1), sizeof(char));
	if (!ret)
		return (NULL);
	while (line[i])
	{
		if (line[i] == 34 || line[i] == 39)
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				ret[j++] = line[i++];
		}
		if (!line[i] && quote != 0)
			return (free(ret), NULL);
		else if (line[i] != 34 && line[i] != 39)
			ret[j++] = line[i];
		i++;
	}
	ret[j] = '\0';
	return (ret);
}

int	ft_set_quote(char *str, int *i)
{
	char	quote;

	quote = 0;
	*i = ft_check_whitespace(str, *i);
	if (str[*i] == '"' || str[*i] == '\'')
	{
		quote = str[*i];
		if (quote == '"')
			(*i)++;
	}
	return (quote);
}
