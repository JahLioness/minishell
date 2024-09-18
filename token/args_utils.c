/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 14:31:22 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 11:48:45 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_skip_quote(char *str, int i)
{
	char	quote;

	if (str[i] == '"' || str[i] == '\'')
	{
		quote = str[i];
		i++;
		while (str[i] && str[i] != quote)
			i++;
	}
	return (i);
}

int	ft_countwords_args(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			count++;
			i = ft_skip_quote(str, i);
		}
		while (str[i] && ft_is_not_stop(str[i]) && !ft_is_whitespaces(str[i])
			&& str[i] != '"' && str[i] != '\'')
			i++;
		if (str[i])
			i++;
		count++;
		if (!ft_is_not_stop(str[i]))
			break ;
	}
	return (count);
}

static int	ft_check_key_arg(char *str, t_cmd *cmd)
{
	int	i;

	i = 2;
	while (str[i] && str[i] != 125)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			cmd->error = 1;
			break ;
		}
		i++;
	}
	return (i);
}

static char	*ft_check_value_from_env(char *key, char *value, t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			value = ft_strdup(tmp->value);
			break ;
		}
		tmp = tmp->next;
	}
	free(key);
	return (value);
}

char	*ft_check_acc_expand(char *str, t_env **env, t_cmd *cmd, int j)
{
	int		i;
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	if (str[0] != '$')
		return (ft_check_expand(str, env, j));
	i = ft_check_key_arg(str, cmd);
	if (cmd->error)
	{
		ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(1)));
		value = ft_strdup("minishell: ");
		value = ft_strjoin_free(value, str);
		value = ft_strjoin_free(value, ": bad substitution");
		cmd->error = 0;
	}
	else
	{
		key = ft_substr(str, 2, i - 2);
		value = ft_check_value_from_env(key, value, env);
	}
	return (value);
}
