/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:30:10 by ede-cola          #+#    #+#             */
/*   Updated: 2023/11/06 12:13:02 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (((char *)dest) != ((char *)src))
	{
		if (((char *)src) > ((char *)dest))
		{
			while (i < n)
			{
				((char *)dest)[i] = ((char *)src)[i];
				i++;
			}
		}
		else
		{
			i = n;
			while (i > 0)
			{
				((char *)dest)[i - 1] = ((char *)src)[i - 1];
				i--;
			}
		}
	}
	return (dest);
}
