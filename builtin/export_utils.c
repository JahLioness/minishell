/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:51:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/25 12:15:40 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_op_egal(char *key, char *value, t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			free(tmp->value);
			if (value)
				tmp->value = ft_strdup(value);
			else
				tmp->value = NULL;
			return ;
		}
		tmp = tmp->next;
	}
	if (value)
		ft_value_len(value, key, env);
	else
		ft_envadd_back(env, ft_envnew(ft_strndup(key, ft_strlen(key)), value));
}

static void	ft_op_plus(char *key, char *value, t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (value)
			{
				if (tmp->value)
					tmp->value = ft_strjoin_free(tmp->value, value);
				else
					tmp->value = ft_strdup(value);
			}
			return ;
		}
		tmp = tmp->next;
	}
	if (value)
		ft_envadd_back(env, ft_envnew(ft_strdup(key), ft_strdup(value)));
	else
		ft_envadd_back(env, ft_envnew(ft_strdup(key), value));
}

char	*ft_check_key(char *key)
{
	int	i;

	i = 0;
	if (!key)
		return (NULL);
	while (key[i])
	{
		if ((i == 0 && ft_isdigit(key[i])) || (!ft_isalnum(key[i])
				&& key[i] != '_'))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(key, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return ("error");
		}
		i++;
	}
	return (key);
}

static void	ft_gestion_erreur(char *key, char *op, char *value, t_env **env)
{
	t_env	*status;

	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(key, 2);
	ft_putstr_fd(op, 2);
	if (value)
	{
		ft_putstr_fd(value, 2);
		free(value);
	}
	ft_putendl_fd(": not a valid identifier'", 2);
	status = ft_get_exit_status(env);
	if (status)
		ft_change_exit_status(status, ft_itoa(1));
	else
		ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(1)));
}

int	ft_check_value(t_env **env, char *key, char *op, char *value)
{
	t_env	*tmp;

	tmp = *env;
	if (op == NULL)
	{
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, key))
				return (0);
			tmp = tmp->next;
		}
		ft_envadd_back(env, ft_envnew(ft_strdup(key), value));
	}
	else if (!ft_strcmp(op, "="))
		ft_op_egal(key, value, env);
	else if (!ft_strcmp(op, "+="))
		ft_op_plus(key, value, env);
	else
	{
		ft_gestion_erreur(key, op, value, env);
		return (1);
	}
	free(value);
	return (0);
}
