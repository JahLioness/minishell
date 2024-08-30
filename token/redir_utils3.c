/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:14:04 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/30 18:16:00 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_loop_get_redir_arg(t_cmd *new, char *cell, int *i, int j)
{
	int		k;

	while (cell[*i] && ft_is_not_stop(cell[*i]))
	{
		if (cell[*i])
		{
			k = ft_get_index_arg_utils(cell, i);
			new->args[j] = ft_strndup(cell + k, (*i - k));
			j++;
		}
		if (cell[*i] && ft_is_not_stop(cell[*i]) && cell[*i] != '$'
			&& cell[*i] != 123 && cell[*i] != '"' && cell[*i] != '\''
			&& !ft_isalnum(cell[*i]))
			(*i)++;
	}
}

void	ft_check_redir_arg(t_cmd *new, char *cell, int *i)
{
	int		j;
	char	**new_args;

	j = 0;
	if (!new || !new->args)
		return ;
	new_args = ft_calloc(ft_tab_len(new->args) + 2, sizeof(char *));
	while (new->args && new->args[j])
	{
		new_args[j] = ft_strdup(new->args[j]);
		j++;
	}
	ft_free_tab(new->args);
	new->args = new_args;
	ft_loop_get_redir_arg(new, cell, i, j);
}
