/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 15:47:09 by ede-cola          #+#    #+#             */
/*   Updated: 2024/05/06 13:39:33 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_strdup_tab(char **s)
{
	int		i;
	char	**ret;

	ret = ft_calloc(ft_tab_len(s) + 1, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	while (s[i])
	{
		ret[i] = ft_strdup(s[i]);
		i++;
	}
	ret[i] = NULL;
	return (ret);
}
