/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_init2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:31:53 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 14:00:55 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_expandable(char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			quote = str[i];
			while (str[++i] && str[i] != quote)
			{
				if (quote == '"' && str[i] == '$')
					return (1);
			}
			if (str[i++] == quote)
				quote = 0;
		}
		if (!quote && str[i] == '$')
			return (1);
		if (str[i] && str[i] != '"' && str[i] != '\'')
			i++;
	}
	return (0);
}

char	*ft_set_expand(char **str, int j, t_env **env)
{
	char	*ret;
	char	*tmp;

	if (ft_is_expandable(str[j]))
		ret = ft_check_expand(str[j], env, j);
	else
		ret = ft_strdup(str[j]);
	tmp = ret;
	while (ret && ft_is_expandable(ret))
	{
		ret = ft_check_expand(tmp, env, j);
		if (!ft_strcmp(tmp, ret))
		{
			free(tmp);
			break ;
		}
		free(tmp);
		tmp = ret;
	}
	return (ret);
}

char	*ft_verif_arg(char **str, t_env **env, t_cmd *cmd, int j)
{
	char	*ret;

	if (!ft_strchr(str[j], '$') || (ft_strchr(str[j], '$')
			&& *(ft_strchr(str[j], '$') + 1) != '?'
			&& !ft_isalnum(*(ft_strchr(str[j], '$') + 1)) && *(ft_strchr(str[j],
					'$') + 1) != '_'))
		return (str[j]);
	else if (str[j][0] == '\'')
		return (str[j]);
	else if (ft_strchr(str[j], 123))
		ret = ft_check_acc_expand(str[j], env, cmd, j);
	else
		ret = ft_set_expand(str, j, env);
	if (!ret)
		return (free(str[j]), ft_strdup(""));
	return (free(str[j]), ret);
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
			if (!args[j])
				break ;
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
