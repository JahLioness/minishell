/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:32:12 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/27 18:05:51 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_skip_betwen_quote(char *str, int *i, char quote)
{
	while (str[*i] && str[*i] != quote)
		(*i)++;
	(*i)++;
}

int	ft_skip_spaces(char *str, int *i)
{
	while (str[*i] && ft_is_whitespaces(str[*i]))
		(*i)++;
	return (*i);
}

char	**ft_free_envp(t_exec_utils *e_utils)
{
	if (e_utils->envp)
		ft_free_tab(e_utils->envp);
	return (NULL);
}
