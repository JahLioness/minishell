/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 14:49:37 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 12:07:27 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen_gnl(char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (0);
	while (str[len])
		len++;
	return (len);
}

char	*ft_check_str(char *str)
{
	if (!str)
	{
		str = malloc(1);
		if (!str)
			return (NULL);
		str[0] = '\0';
	}
	return (str);
}

char	*ft_strndup(char *s, size_t n)
{
	size_t	len;
	size_t	i;
	char	*ret;

	i = 0;
	if (!s || n == 0)
		return (NULL);
	len = ft_strlen(s);
	if (len == 0)
		return (NULL);
	if (n > len)
		n = len;
	ret = malloc(sizeof(char) * (n + 1));
	if (!ret)
		return (NULL);
	while (i < n)
	{
		ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

char	*ft_strchr_gnl(char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (s[i] == (unsigned char)c)
			return (&s[i]);
		i++;
	}
	if (s[i] == '\0' && (unsigned char)c == '\0')
		return (&s[i]);
	else
		return (NULL);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*ret;

	i = 0;
	j = 0;
	s1 = ft_check_str(s1);
	if (!s1 || !s2)
		return (free(s1), NULL);
	ret = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!ret)
		return (free(s1), NULL);
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	while (s2[j])
		ret[i++] = s2[j++];
	ret[i] = '\0';
	free(s1);
	return (ret);
}
