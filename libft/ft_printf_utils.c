/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:43:21 by ede-cola          #+#    #+#             */
/*   Updated: 2024/01/29 14:03:15 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen_printf(const char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

size_t	ft_nbr_length(int n)
{
	size_t	count;

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

size_t	ft_unsigned_length(unsigned long long n, int base)
{
	size_t	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n = n / base;
		count++;
	}
	return (count);
}

void	ft_putnbr_unsigned(unsigned long long n)
{
	char	c;

	if (n > 9)
		ft_putnbr_unsigned(n / 10);
	c = '0' + (n % 10);
	write(1, &c, 1);
}

void	ft_putnbr16(unsigned long long n, char format)
{
	char	c;
	char	*digits;

	digits = "0123456789abcdef";
	if (n >= 16)
		ft_putnbr16(n / 16, format);
	if (format == 'X')
		c = ft_toupper(digits[n % 16]);
	else
		c = digits[n % 16];
	write(1, &c, 1);
}
