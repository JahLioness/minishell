/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_deal_format_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 18:52:04 by ede-cola          #+#    #+#             */
/*   Updated: 2023/12/15 15:41:59 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_deal_xformat(unsigned int nb, char format)
{
	ft_putnbr16(nb, format);
	return (ft_unsigned_length(nb, 16));
}
