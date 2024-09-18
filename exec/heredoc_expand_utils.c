/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:09:53 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 15:39:46 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_search_value_heredoc(char *ret, t_env **env, int i, char *str)
{
	t_env	*tmp;
	int		found;

	tmp = *env;
	found = 0;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, str + i, ft_strlen(tmp->key))
			&& ft_check_len(str, i, tmp->key))
		{
			if (tmp->value)
			{
				ret = ft_get_ret_heredoc(tmp, ret, str, i);
				return (ret);
			}
			found = 1;
			break ;
		}
		tmp = tmp->next;
	}
	if (!found)
		ret = ft_get_quotes(str, ret, i);
	return (ret);
}

static int	ft_is_expandable_heredoc(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		if (str[i])
			i++;
	}
	return (0);
}

static char	*ft_check_expand_heredoc(char *str, t_env **env, int j)
{
	int		i;
	char	*ret;

	i = ft_get_index_ba_var(str, 0);
	if (!ft_strchr(str, '$') || !ft_is_expandable_heredoc(str))
		return (ft_strdup(str));
	if (str[i] == '$' && ((!ft_isalnum(str[i + 1]) && str[i + 1] != '?' && str[i
					+ 1] != '_') || (j == 0 && str[i + 1] == '_')))
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
		return (NULL);
	i++;
	ret = ft_search_value_heredoc(ret, env, i, str);
	return (ret);
}

static char	*ft_set_expand_heredoc(char **str, int j, t_env **env)
{
	char	*ret;
	char	*tmp;

	if (ft_is_expandable_heredoc(str[j]))
		ret = ft_check_expand_heredoc(str[j], env, j);
	else
		ret = ft_strdup(str[j]);
	tmp = ret;
	while (ret && ft_is_expandable_heredoc(ret))
	{
		ret = ft_check_expand_heredoc(tmp, env, j);
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

char	*ft_verif_arg_heredoc(char **str, t_env **env, t_cmd *cmd, int j)
{
	char	*ret;

	if (!ft_strchr(str[j], '$') || (ft_strchr(str[j], '$')
			&& *(ft_strchr(str[j], '$') + 1) != '?'
			&& !ft_isalnum(*(ft_strchr(str[j], '$') + 1)) && *(ft_strchr(str[j],
					'$') + 1) != '_'))
		return (str[j]);
	else if (ft_strchr(str[j], 123))
		ret = ft_check_acc_expand(str[j], env, cmd, j);
	else
		ret = ft_set_expand_heredoc(str, j, env);
	if (!ret)
		return (free(str[j]), ft_strdup(""));
	return (free(str[j]), ret);
}
