/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:10:22 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/24 19:10:55 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_check_flag(char **flag)
{
	int	i;
	int	j;

	i = 0;
	while (flag[i])
	{
		j = 1;
		if (flag[i][0] != '-')
			return (i);
		while (flag[i][j])
		{
			if (flag[i][j] != 'n')
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

void	ft_echo_wt_flag(int fd, char **str, char **flag)
{
	size_t	flag_index;
	size_t	i;

	flag_index = ft_check_flag(flag);
	i = flag_index;
	while (i < ft_tab_len(flag))
	{
		ft_putstr_fd(flag[i], fd);
		write(fd, " ", 1);
		i++;
	}
	if (flag_index > 0)
		ft_echo_putstr(fd, str);
	else
		ft_echo_putendl(fd, str);
}
