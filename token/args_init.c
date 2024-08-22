/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:08:17 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/22 13:10:40 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_check_acco(char *str, int *i)
{
	if (str[*i] == 123)
	{
		(*i)++;
		while (str[*i] && str[*i] != 125)
			(*i)++;
	}
}

int	ft_get_index_arg_utils(char *str, int *i)
{
	int		k;
	char	quote;

	k = *i;
	quote = 0;
	while (str[*i] && !ft_is_whitespaces(str[*i]) && ft_is_not_stop(str[*i])
		&& !quote)
	{
		if (str[*i] == '"' || str[*i] == '\'')
		{
			quote = str[*i];
			(*i)++;
			while (str[*i] && str[*i] != quote)
				(*i)++;
			if (str[*i] == quote)
				(*i)++;
			if (!ft_is_whitespaces(str[*i]))
				ft_get_index_arg_utils(str, i);
		}
		else
			ft_check_acco(str, i);
		if (str[*i])
			(*i)++;
	}
	return (k);
}

int	ft_get_index_ba_var(char *str, int i)
{
	while (str[i])
	{
		if (str[i] == '$')
			break ;
		i++;
	}
	return (i);
}

char	*ft_search_value(char *ret, t_env **env, int i, char *str)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, str + i, ft_strlen(tmp->key)))
		{
			if (tmp->value)
			{
				if (!ft_strcmp(tmp->key, "_"))
				{
					ret = ft_strjoin_free(ret, ft_strrchr(tmp->value, '/') + 1);
					ret = ft_strjoin_free(ret, str + i + ft_strlen(tmp->key));
					return (ret);
				}
				ret = ft_strjoin_free(ret, tmp->value);
				ret = ft_strjoin_free(ret, str + i + ft_strlen(tmp->key));
				return (ret);
			}
			break ;
		}
		tmp = tmp->next;
	}
	free(ret);
	return (NULL);
}

char	*ft_check_expand(char *str, t_env **env, int j)
{
	int		i;
	char	*ret;

	i = ft_get_index_ba_var(str, 0);
	if (!ft_strchr(str, '$'))
		return (str);
	if (str[i] == '$' && ((!ft_isalnum(str[i + 1]) && str[i + 1] != '?'
				&& str[i + 1] != '_') || (j == 0 && str[i + 1] == '_')))
	{
		ret = ft_strdup(str);
		if (!ret)
			return (NULL);
		return (ret);
	}
	if (i > 0)
		ret = ft_strndup(str, i);
	else
		ret = ft_strdup("");
	if (!ret)
		return (free(str), NULL);
	i++;
	ret = ft_search_value(ret, env, i, str);
	return (ret);
}

// int	ft_get_index_ba_var(char *str, int i)
// {
// 	char	quote;

// 	quote = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '"' || str[i] == '\'')
// 		{
// 			quote = str[i];
// 			i++;
// 			if (str[i] == '$' && quote != '\'')
// 				break ;
// 			while (str[i] && str[i] != quote)
// 				i++;
// 			if (str[i] == quote)
// 				i++;
// 			quote = 0;
// 		}
// 		if (str[i] == '$' && quote != '\'')
// 			break ;
// 		if (str[i] && str[i] != '\'' && str[i] != '"')
// 			i++;
// 	}
// 	return (i);
// }
// int	ft_get_index_arg_utils(char *str, int *i)
// {
// 	int		k;
// 	char	quote;

// 	k = *i;
// 	if (str[*i] == '"' || str[*i] == '\'')
// 	{
// 		quote = str[*i];
// 		(*i)++;
// 		while (str[*i] && str[*i] != quote)
// 			(*i)++;
// 		if (str[*i] == quote)
// 			(*i)++;
// 		if (!ft_is_whitespaces(str[*i]))
// 			ft_get_index_arg_utils(str, i);
// 	}
// 	else
// 	{
// 		while (str[*i] && ft_is_not_stop(str[*i])
// 			&& !ft_is_whitespaces(str[*i]))
// 		{
// 			ft_check_acco(str, i);
// 			(*i)++;
// 		}
// 	}
// 	return (k);
// }
