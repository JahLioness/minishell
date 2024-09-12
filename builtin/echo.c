/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:56:42 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/12 15:09:12 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_check_echo_tab(char **str, int i, int fd)
{
	int	j;

	j = 0;
	while (str[i][j])
	{
		if (j == 0 && str[i][j] == '{' && ft_strchr(str[i], ','))
		{
			j++;
			while (str[i][j] && str[i][j] != '}')
			{
				if (str[i][j] == ',')
				{
					write(fd, " ", 1);
					j++;
				}
				write(fd, &str[i][j++], 1);
			}
			j++;
		}
		else
		{
			ft_putstr_fd(str[i], fd);
			break ;
		}
	}
}

void	ft_echo_putendl(int fd, char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		ft_check_echo_tab(str, i, fd);
		if (str[i + 1])
			write(fd, " ", 1);
		else
			write(fd, "\n", 1);
		i++;
	}
}

void	ft_echo_putstr(int fd, char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		ft_check_echo_tab(str, i, fd);
		if (str[i + 1])
			write(fd, " ", 1);
		i++;
	}
}

int	ft_echo(int fd, char **str, char **flag)
{
	if ((!str || !*str) && !flag)
	{
		ft_putendl_fd("", fd);
		free(str);
		return (0);
	}
	else if (!flag)
		ft_echo_putendl(fd, str);
	else
		ft_echo_wt_flag(fd, str, flag);
	ft_free_tab(str);
	if (flag)
		ft_free_tab(flag);
	return (0);
}
