/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_init2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:31:53 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/09 14:52:58 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_verif_arg(char **str, t_env **env, t_cmd *cmd, int j)
{
	char	*ret;
	char	*tmp;

	if (!ft_strchr(str[j], '$') || (ft_strchr(str[j], '$')
			&& *(ft_strchr(str[j], '$') + 1) != '?'
			&& !ft_isalnum(*(ft_strchr(str[j], '$') + 1))))
		return (str[j]);
	else if (str[j][0] == '\'')
		return (str[j]);
	else if (ft_strchr(str[j], 123))
		ret = ft_check_acc_expand(str[j], env, cmd, j);
	else
	{
		ret = ft_check_expand(str[j], env, j);
		tmp = ret;
		while (ft_strchr(ret, '$'))
		{
			ret = ft_check_expand(tmp, env, j);
			free(tmp);
			tmp = ret;
		}
	}
	free(str[j]);
	if (!ret)
		return (ft_strdup(""));
	return (ret);
}

char	**ft_get_args(char *cell, int *i)
{
	char	**args;
	int		j;
	int		k;

	args = ft_calloc(ft_countwords_args(cell + *i) + 1, sizeof(char *));
	if (!args)
		return (NULL);
	j = 0;
	while (cell[*i] && ft_is_not_stop(cell[*i]))
	{
		if (cell[*i])
		{
			k = ft_get_index_arg_utils(cell, i);
			args[j] = ft_strndup(cell + k, (*i - k));
			// printf("args[%d] = %s\n", j, args[j]);
			if (!args[j])
				return (ft_free_split(j, args), NULL);
			j++;
		}
		if (cell[*i] && ft_is_not_stop(cell[*i]) && cell[*i] != '$'
			&& cell[*i] != 123 && cell[*i] != '"' && cell[*i] != '\''
			&& !ft_isalnum(cell[*i]))
			(*i)++;
	}
	args[j] = NULL;
	return (args);
}
