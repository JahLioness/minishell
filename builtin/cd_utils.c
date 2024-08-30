/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:53:55 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/30 14:02:13 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_old_pwd(t_env **env)
{
	t_env	*tmp;
	char	*old_pwd;

	tmp = *env;
	old_pwd = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "OLDPWD"))
		{
			old_pwd = tmp->value;
			break ;
		}
		tmp = tmp->next;
	}
	return (old_pwd);
}

char	*ft_get_home(t_env **env)
{
	t_env	*tmp;
	char	*home;

	tmp = *env;
	home = NULL;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "HOME"))
		{
			home = tmp->value;
			break ;
		}
		tmp = tmp->next;
	}
	return (home);
}

char	*ft_gestion_tilde(t_env **env, char *new_dir)
{
	char	*tmp_dir;
	char	*tmp_new;
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "HOME"))
		{
			if (ft_strlen(new_dir) == 1)
				new_dir = ft_strdup(tmp->value);
			else
			{
				tmp_dir = ft_strdup_free(new_dir);
				new_dir = ft_strchr(tmp_dir, '/');
				tmp_new = ft_strdup(new_dir);
				free(tmp_dir);
				new_dir = ft_strjoin(tmp->value, tmp_new);
				free(tmp_new);
			}
			break ;
		}
		tmp = tmp->next;
	}
	return (new_dir);
}
