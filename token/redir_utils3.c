/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:14:04 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 11:49:10 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_check_char(char c)
{
	if (c != '$' && c != 123 && c != '"' && c != '\'' && !ft_isalnum(c)
		&& ft_is_not_stop(c))
		return (1);
	return (0);
}

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

static void	ft_copy_args(t_cmd *new, char **new_args, int *j)
{
	while (new_args && new_args[*j])
	{
		new->args[*j] = ft_strdup(new_args[*j]);
		(*j)++;
	}
}

static void	ft_realloc_args(t_cmd *new, char **new_args)
{
	ft_free_tab(new->args);
	new->args = ft_calloc(ft_tab_len(new_args) + 2, sizeof(char *));
}

void	ft_check_redir_arg(t_cmd *new, char *cell, int *i)
{
	int		j;
	int		k;
	char	**new_args;

	if (!new || !new->args)
		return ;
	new_args = ft_tab_copy(new->args);
	while (cell[*i] && ft_is_not_stop(cell[*i]))
	{
		if (cell[*i])
		{
			ft_realloc_args(new, new_args);
			j = 0;
			ft_copy_args(new, new_args, &j);
			ft_free_tab(new_args);
			k = ft_get_index_arg_utils(cell, i);
			new->args[j] = ft_strndup(cell + k, (*i - k));
			j++;
			new->args[j] = NULL;
			new_args = ft_tab_copy(new->args);
		}
		if (cell[*i] && ft_check_char(cell[*i]))
			(*i)++;
	}
	ft_free_tab(new_args);
}
