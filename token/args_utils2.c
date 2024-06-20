/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:22:24 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/18 15:23:27 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_trim_quote_args(char **args)
{
	int		i;
	char	**ret;

	i = 0;
	if (!args || !*args)
		return (ft_free_tab(args), NULL);
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
			return (ft_free_tab(args), ft_free_split(i, ret), NULL);
		if (args[i])
			i++;
	}
	ret[i] = NULL;
	ft_free_tab(args);
	return (ret);
}
