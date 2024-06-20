/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:58:57 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/17 17:52:44 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_change_env(t_env **env, char *pwd, char *old_dir)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PWD"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(pwd);
		}
		if (!ft_strcmp(tmp->key, "OLDPWD"))
		{
			free(tmp->value);
			tmp->value = ft_strdup(old_dir);
		}
		tmp = tmp->next;
	}
}

int	ft_print_env(t_env **env, int fd)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (tmp->value && ft_strcmp(tmp->key, "?"))
		{
			ft_putstr_fd(tmp->key, fd);
			ft_putstr_fd("=", fd);
			ft_putendl_fd(tmp->value, fd);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_env_delone(t_env *env)
{
	if (!env)
		return ;
	free(env->key);
	if (env->value)
		free(env->value);
	env->next = NULL;
	free(env);
}

void	ft_clearenv(t_env **env)
{
	t_env	*tmp;
	t_env	*next;

	if (!env)
		return ;
	tmp = *env;
	while (tmp)
	{
		next = tmp->next;
		ft_env_delone(tmp);
		tmp = next;
	}
	*env = NULL;
}

t_env	*ft_env_copy(t_env *env)
{
	t_env	*copy;
	t_env	*new_env;
	t_env	*tmp;

	copy = NULL;
	tmp = env;
	while (tmp)
	{
		new_env = (t_env *)malloc(sizeof(t_env));
		if (!new_env)
			return (NULL);
		new_env->key = ft_strdup(tmp->key);
		new_env->value = ft_strdup(tmp->value);
		new_env->next = NULL;
		if (copy == NULL)
			copy = new_env;
		else
		{
			new_env->next = copy;
			copy = new_env;
		}
		tmp = tmp->next;
	}
	return (copy);
}
