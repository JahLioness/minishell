/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 15:32:12 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/06 16:12:18 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_skip_betwen_quote(char *str, int *i, char quote)
{
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i] == quote)
		(*i)++;
}

int	ft_skip_spaces(char *str, int *i)
{
	while (str[*i] && ft_is_whitespaces(str[*i]))
		(*i)++;
	return (*i);
}

void	ft_free_envp(t_exec_utils *e_utils)
{
	if (e_utils->envp)
		ft_free_tab(e_utils->envp);
	e_utils->envp = NULL;
}
