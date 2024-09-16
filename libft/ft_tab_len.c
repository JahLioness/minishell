/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tab_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:49:05 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/16 16:10:31 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_tab_len(char **tab)
{
	size_t	i;

	i = 0;
	if (!tab || !tab[0])
		return (0);
	while (tab[i] && tab[i][0])
	{
		printf("tab[%zu] = %s\n",i ,tab[i]);
		i++;
	}
	return (i);
}
