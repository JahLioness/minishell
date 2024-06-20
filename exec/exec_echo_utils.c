/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:59:46 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/20 13:27:38 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_count_flag(char **args)
{
	int	i;
	int	count;

	i = 1;
	count = 0;
	if (!args || !*args)
		return (0);
	while (args && args[i])
	{
		if (*args[i] && args[i][0] == '-')
			count++;
		i++;
	}
	return (count);
}

char	**ft_get_flag_echo(char **args)
{
	int		i;
	int		j;
	int		count;
	char	**flag;

	count = ft_count_flag(args);
	if (count == 0)
		return (NULL);
	flag = ft_calloc(count + 1, sizeof(char *));
	if (!flag)
		return (NULL);
	i = 0;
	j = 0;
	while (args[++i])
	{
		if (args[i][0] == '-')
		{
			flag[j] = ft_strdup(args[i]);
			if (!flag[j])
				return (ft_free_split(j, flag), NULL);
			j++;
		}
	}
	flag[j] = NULL;
	return (flag);
}

char	**ft_get_args_echo(char **args, t_env **env)
{
	int		i;
	int		j;
	int		count;
	char	**arg;

	(void)env;
	count = ft_count_flag(args);
	arg = ft_calloc(ft_tab_len(args) - count, sizeof(char *));
	if (!arg)
		return (NULL);
	i = 0;
	j = 0;
	while (args[++i])
	{
		if (args[i][0] != '-')
		{
			arg[j] = ft_strdup(args[i]);
			if (!arg[j])
				return (ft_free_split(j, arg), NULL);
			j++;
		}
	}
	arg[j] = NULL;
	return (arg);
}
