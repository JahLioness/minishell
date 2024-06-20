/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:47:38 by ede-cola          #+#    #+#             */
/*   Updated: 2023/12/15 15:58:39 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nbr_length_itoa(int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*ret;
	long	nb;
	int		i;
	int		len;

	len = ft_nbr_length_itoa(n);
	nb = n;
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	ret[len] = '\0';
	if (nb < 0)
	{
		i = 1;
		nb = -nb;
		ret[0] = '-';
	}
	else
		i = 0;
	while (len-- > i)
	{
		ret[len] = '0' + (nb % 10);
		nb /= 10;
	}
	return (ret);
}
