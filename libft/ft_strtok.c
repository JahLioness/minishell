/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:35:24 by ede-cola          #+#    #+#             */
/*   Updated: 2024/05/02 17:36:52 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtok(char *str, char *sep)
{
	static char	*save;
	char		*ret;
	int			i;
	int			j;

	i = 0;
	j = 0;
	if (!str || !*str)
		return (NULL);
	save = str;
	while (save[i] && ft_strchr(sep, save[i]))
		i++;
	while (save[i + j] && !ft_strchr(sep, save[i + j]))
		j++;
	ret = malloc(sizeof(char) * (j + 1));
	if (!ret)
		return (NULL);
	j = 0;
	while (save[i] && !ft_strchr(sep, save[i]))
		ret[j++] = save[i++];
	ret[j] = '\0';
	while (save[i] && ft_strchr(sep, save[i]))
		i++;
	save += i;
	return (ret);
}
