/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:58:41 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/23 17:27:43 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_flags_len(char **flags)
{
	int	i;
	int	j;
	int	len;

	len = 0;
	i = 1;
	while (flags && flags[i])
	{
		if (*flags[i] && flags[i][0] == '-')
		{
			j = 1;
			while (flags[i][j])
			{
				if (flags[i][j] == 'n')
					j++;
				else
					break ;
			}
			if (j == (int)ft_strlen(flags[i]))
				len++;
		}
		i++;
	}
	return (len);
}

char	*ft_get_arg_echo_loop(char *arg)
{
	int		i;
	char	*ret;

	if (arg[0] == '-')
	{
		i = 1;
		while (arg[i] == 'n')
			i++;
		if (i != (int)ft_strlen(arg))
		{
			ret = ft_strdup(arg);
			if (!ret)
				return (NULL);
			return (ret);
		}
	}
	else
	{
		ret = ft_strdup(arg);
		if (!ret)
			return (NULL);
		return (ret);
	}
	return ("");
}
