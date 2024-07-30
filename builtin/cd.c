/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:00:19 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/25 21:41:40 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_cd_utils(t_env **env, char *old_dir)
{
	t_env	*tmp;
	char	*pwd;

	tmp = *env;
	pwd = getcwd(NULL, 0);
	if (!tmp)
	{
		ft_envadd_back(env, ft_envnew(ft_strdup("PWD"), ft_strdup(pwd)));
		ft_envadd_back(env, ft_envnew(ft_strdup("OLDPWD"), ft_strdup(old_dir)));
	}
	else
		ft_change_env(env, pwd, old_dir);
	free(pwd);
}

static char	*ft_get_new_dir(char *old_dir, char *new_dir)
{
	if (!new_dir)
	{
		new_dir = ft_strndup(old_dir, (ft_strlen(old_dir)
					- ft_strlen(ft_strchr(ft_strchr(old_dir, '/') + 6, '/'))));
	}
	return (new_dir);
}
int	ft_cd_util(char **path, t_env **env)
{
	char	*new_dir;
	char	*old_dir;
	t_env	*status;
	
	new_dir = ft_strdup(path[1]);
	old_dir = getcwd(NULL, 0);
	new_dir = ft_get_new_dir(old_dir, new_dir);
	if (chdir(new_dir) == 0)
		ft_cd_utils(env, old_dir);
	else
	{
		status = ft_get_exit_status(env);
		if (status)
			ft_change_exit_status(status, ft_itoa(1));
		else
			ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(1)));
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(new_dir, 2);
		ft_putendl_fd(": No such file or directory", 2);
		return (free(new_dir), free(old_dir), 1);
	}
	return (free(new_dir), free(old_dir), 0);
}

int	ft_cd(char **path, t_env **env)
{
	t_env	*status;

	if (ft_tab_len(path) > 2)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putendl_fd("too many arguments", 2);
		status = ft_get_exit_status(env);
		if (status)
			ft_change_exit_status(status, ft_itoa(1));
		else
			ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(1)));
		return (1);
	}
	else
		return (ft_cd_util(path, env));
}
