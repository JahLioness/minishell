/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:43:26 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/25 12:14:40 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sort_list(t_env **sorted_env, t_env *tmp, t_env *prev)
{
	t_env	*sorted;

	while (tmp->next)
	{
		sorted = tmp->next;
		if (ft_strcmp(tmp->key, sorted->key) > 0)
		{
			if (prev == NULL)
				*sorted_env = sorted;
			else
				prev->next = sorted;
			tmp->next = sorted->next;
			sorted->next = tmp;
			tmp = sorted;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

static t_env	*ft_alpha_sorted_env(t_env **env)
{
	t_env	*tmp;
	t_env	*sorted_env;
	t_env	*prev;
	int		size;

	size = ft_envsize(*env);
	sorted_env = ft_env_copy(*env);
	prev = NULL;
	while (size > 1)
	{
		tmp = sorted_env;
		prev = NULL;
		sort_list(&sorted_env, tmp, prev);
		size--;
	}
	return (sorted_env);
}

static void	ft_print_export(t_env *env, int fd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->value && ft_strcmp(tmp->key, "?"))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(tmp->key, fd);
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd(tmp->value, fd);
			ft_putendl_fd("\"", fd);
		}
		else if (ft_strcmp(tmp->key, "?"))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putendl_fd(tmp->key, fd);
		}
		tmp = tmp->next;
	}
}

int	ft_export(t_export_utils *utils, t_env **env, int fd)
{
	t_env	*export;
	t_env	*status;

	if (!ft_strcmp(ft_check_key(utils->key), "error"))
	{
		status = ft_get_exit_status(env);
		if (status)
			ft_change_exit_status(status, ft_itoa(1));
		else
			ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(1)));
		return (1);
	}
	else if (ft_check_key(utils->key) && ft_strcmp(ft_check_key(utils->key),
			"error"))
		return (ft_check_value(env, utils->key, utils->op, utils->value));
	else
	{
		if (!env || !*env)
			return (1);
		export = ft_alpha_sorted_env(env);
		ft_print_export(export, fd);
		ft_clearenv(&export);
	}
	free(utils->value);
	return (0);
}

void	ft_value_len(char *value, char *key, t_env **env)
{
	if (ft_strlen(value) == 0)
		ft_envadd_back(env, ft_envnew(ft_strndup(key, ft_strlen(key)),
				ft_strdup(value)));
	else
		ft_envadd_back(env, ft_envnew(ft_strndup(key, ft_strlen(key)),
				ft_strndup(value, ft_strlen(value))));
}
