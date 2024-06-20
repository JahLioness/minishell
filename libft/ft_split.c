/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:30:54 by ede-cola          #+#    #+#             */
/*   Updated: 2024/05/10 16:38:00 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_countword(char const *s, char c)
{
	int	count;

	count = 0;
	while (s && *s)
	{
		if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else if (*s == c)
			s++;
	}
	return (count);
}

static int	ft_countwordsize(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

void	ft_free_split(size_t i, char **split)
{
	while (i > 0)
	{
		free(split[i]);
		i--;
	}
	free(split[i]);
	free(split);
}

static char	**ft_setsplit(char const *s, char c, char **split, size_t count)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < count)
	{
		while (s[j] && s[j] == c)
			j++;
		split[i] = ft_substr(s, j, ft_countwordsize(&s[j], c));
		if (!split[i])
		{
			ft_free_split(i, split);
			return (NULL);
		}
		while (s[j] && s[j] != c)
			j++;
		i++;
	}
	split[i] = NULL;
	return (split);
}

char	**ft_split(const char *s, char c)
{
	char	**ret;
	size_t	count;

	if (!s)
		return (NULL);
	count = ft_countword(s, c);
	ret = malloc(sizeof(char *) * (count + 1));
	if (!ret)
		return (NULL);
	ret = ft_setsplit(s, c, ret, count);
	return (ret);
}
