/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tab_copy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 10:45:01 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/17 10:53:18 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_tab_copy(char **tab)
{
	char	**new;
	size_t	i;

	i = 0;
	new = ft_calloc(ft_tab_len(tab) + 1, sizeof(char *));
	if (!new)
		return (NULL);
	while (tab && tab[i])
	{
		new[i] = ft_strdup(tab[i]);
		i++;
	}
	new[i] = NULL;
	return (new);
}
