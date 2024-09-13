/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:46:31 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/12 10:54:55 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_unset(t_cmd *cmd, t_env **env, int fd)
{
	char	**args;
	int		i;

	i = 1;
	(void)fd;
	args = cmd->args;
	if (!args[i])
		return (0);
	while (args[i])
	{
		ft_unset(args[i], env);
		i++;
	}
	return (0);
}
