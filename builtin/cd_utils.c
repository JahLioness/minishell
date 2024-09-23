/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:53:55 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/23 17:03:04 by ede-cola         ###   ########.fr       */
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
			old_pwd = ft_strdup(tmp->value);
			break ;
		}
		tmp = tmp->next;
	}
	return (old_pwd);
}

char	*ft_get_pwd(t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "PWD"))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
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
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "HOME"))
		{
			if (ft_strlen(new_dir) == 1)
			{
				free(new_dir);
				new_dir = ft_strdup(tmp->value);
			}
			else
				new_dir = ft_get_dir(new_dir, tmp->value);
			break ;
		}
		tmp = tmp->next;
	}
	return (new_dir);
}

void	ft_old_inexistent(t_env **env)
{
	char	*tmp_old;
	char	*pwd;

	pwd = ft_get_pwd(env);
	tmp_old = getcwd(NULL, 0);
	if (!tmp_old && ((pwd && ft_strncmp(pwd + 1, "sgoinfre",
					ft_strlen("sgoinfre"))) || !pwd))
		ft_no_oldir(env);
	free(tmp_old);
}
