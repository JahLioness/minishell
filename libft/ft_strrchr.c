/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:57:03 by ede-cola          #+#    #+#             */
/*   Updated: 2023/11/08 14:21:13 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	if (s[i] == '\0' && (unsigned char)c == '\0')
		return (&((char *)s)[i]);
	else
	{
		while (i >= 0)
		{
			if (s[i] == (unsigned char)c)
				return (&((char *)s)[i]);
			i--;
		}
	}
	return (NULL);
}
