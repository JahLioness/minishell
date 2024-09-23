/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 18:59:46 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/23 17:26:58 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_trim_quote_echo(char **args)
{
	int		i;
	char	**ret;

	i = 0;
	if (!args || !*args)
		return (NULL);
	ret = ft_calloc((ft_tab_len(args) + 1), sizeof(char *));
	if (!ret)
		return (NULL);
	while (args[i])
	{
		if (!ft_strncmp(args[i], "minishell:", 10))
			ret[i] = ft_strdup(args[i]);
		else
			ret[i] = ft_trim_quote(args[i], 0, 0);
		if (!ret[i])
			return (ft_free_split(i - 1, ret), NULL);
		if (args[i])
			i++;
	}
	ret[i] = NULL;
	return (ret);
}

int	ft_count_flag(char **args)
{
	int		count;
	char	**flags;

	if (!args || !*args)
		return (0);
	flags = ft_trim_quote_echo(args);
	count = ft_flags_len(flags);
	ft_free_tab(flags);
	return (count);
}

static char	**ft_get_flag(char **args, char **flag)
{
	int		i;
	int		j;
	int		len;

	i = 0;
	j = 0;
	while (args[++i])
	{
		if (args[i][0] == '-')
		{
			len = 1;
			while (args[i][len] == 'n')
				len++;
			if (len == (int)ft_strlen(args[i]))
			{
				flag[j] = ft_strdup(args[i]);
				if (!flag[j])
					return (ft_free_split(j, flag), NULL);
				j++;
			}
		}
	}
	flag[j] = NULL;
	return (flag);
}

char	**ft_get_flag_echo(char **args)
{
	int		count;
	char	**flag;
	char	**tmp_args;

	count = ft_count_flag(args);
	if (count == 0)
		return (NULL);
	tmp_args = ft_trim_quote_echo(args);
	if (!tmp_args)
		return (NULL);
	flag = ft_calloc(count + 1, sizeof(char *));
	if (!flag)
		return (NULL);
	flag = ft_get_flag(tmp_args, flag);
	ft_free_tab(tmp_args);
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
		arg[j] = ft_get_arg_echo_loop(args[i]);
		if (!arg[j])
			return (ft_free_split(j, arg), NULL);
		else if (*arg[j])
			j++;
	}
	arg[j] = NULL;
	return (arg);
}
