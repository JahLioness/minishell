/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:14:04 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/17 11:57:54 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static void	ft_loop_get_redir_arg(t_cmd *new, char *cell, int *i, int j)
// {
// 	int		k;

// 	while (cell[*i] && ft_is_not_stop(cell[*i]))
// 	{
// 		if (cell[*i])
// 		{
// 			k = ft_get_index_arg_utils(cell, i);
// 			new->args[j] = ft_strndup(cell + k, (*i - k));
// 			j++;
// 		}
// 		if (cell[*i] && ft_is_not_stop(cell[*i]) && cell[*i] != '$'
// 			&& cell[*i] != 123 && cell[*i] != '"' && cell[*i] != '\''
// 			&& !ft_isalnum(cell[*i]))
// 			(*i)++;
// 	}
// 	new->args[j] = NULL;
// }



void	ft_check_redir_arg(t_cmd *new, char *cell, int *i)
{
	int		j;
	int 	k;
	char	**new_args;

	j = 0;
	if (!new || !new->args)
		return ;
	new_args = ft_tab_copy(new->args);
	while (cell[*i] && ft_is_not_stop(cell[*i]))
	{
		if (cell[*i])
		{
			ft_free_tab(new->args);
			new->args = ft_calloc(ft_tab_len(new_args) + 2, sizeof(char *));
			j = 0;
			while (new_args && new_args[j])
			{
				new->args[j] = ft_strdup(new_args[j]);
				j++;
			}
			ft_free_tab(new_args);
			k = ft_get_index_arg_utils(cell, i);
			new->args[j] = ft_strndup(cell + k, (*i - k));
			j++;
			new->args[j] = NULL;
			new_args = ft_tab_copy(new->args);
		}
		if (cell[*i] && ft_is_not_stop(cell[*i]) && cell[*i] != '$'
			&& cell[*i] != 123 && cell[*i] != '"' && cell[*i] != '\''
			&& !ft_isalnum(cell[*i]))
			(*i)++;
	}
	ft_free_tab(new_args);
}
