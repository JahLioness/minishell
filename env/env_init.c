/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:17:29 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/25 12:07:52 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_envsize(t_env *env)
{
	int		size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

t_env	*ft_envnew(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

t_env	*ft_envlast(t_env *env)
{
	t_env	*tmp;

	if (!env)
		return (NULL);
	tmp = env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_envadd_back(t_env **env, t_env *new)
{
	t_env	*last;

	if (!env || !new)
		return ;
	if (!*env)
		*env = new;
	else
	{
		last = ft_envlast(*env);
		last->next = new;
	}
}

t_env	*ft_get_env(char **envp)
{
	t_env	*env;
	int		i;

	i = 0;
	env = NULL;
	while (envp[i])
	{
		ft_envadd_back(&env, ft_envnew(ft_strndup(envp[i], ft_strchr(envp[i],
						'=') - envp[i]), ft_strdup(ft_strchr(envp[i], '=')
					+ 1)));
		i++;
	}
	ft_envadd_back(&env, ft_envnew(ft_strdup("?"), ft_itoa(0)));
	return (env);
}
