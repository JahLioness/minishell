/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:15:03 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/25 12:03:48 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_export_utils(char *arg, t_export_utils *utils)
{
	int		i;

	i = 0;
	while (arg[i])
	{
		if (i != 0 && ((arg[i] == '=' && arg[i - 1] != '+') || (arg[i] == '+'
					&& arg[i + 1] == '=')))
		{
			utils->key = ft_substr(arg, 0, i);
			if (arg[i] == '=')
				utils->op = ft_substr(arg, i, 1);
			else
				utils->op = ft_substr(arg, i, 2);
			utils->value = ft_strdup(arg + i + ft_strlen(utils->op));
			break ;
		}
		i++;
	}
	if (!utils->key)
		utils->key = ft_strdup(arg);
	return (0);
}
