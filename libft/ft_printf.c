/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 11:50:01 by ede-cola          #+#    #+#             */
/*   Updated: 2023/12/15 15:42:17 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_deal_format(char format, va_list args)
{
	if (format == 'c')
		return (ft_deal_cformat(va_arg(args, int)));
	else if (format == 's')
		return (ft_deal_sformat(va_arg(args, char *)));
	else if (format == 'p')
		return (ft_deal_pformat(va_arg(args, uintptr_t)));
	else if (format == 'd' || format == 'i')
		return (ft_deal_iformat(va_arg(args, int)));
	else if (format == 'u')
		return (ft_deal_uformat(va_arg(args, unsigned int)));
	else if (format == 'x')
		return (ft_deal_xformat(va_arg(args, unsigned int), 'x'));
	else if (format == 'X')
		return (ft_deal_xformat(va_arg(args, unsigned int), 'X'));
	else
		return (ft_deal_cformat(format));
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	size_t	count;

	count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format != '%')
		{
			ft_putchar_fd(*format, 1);
			count++;
		}
		else if (*format == '%')
		{
			count += ft_deal_format(*(format + 1), args);
			format++;
		}
		format++;
	}
	va_end(args);
	return (count);
}
