/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:00:19 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/23 15:53:06 by ede-cola         ###   ########.fr       */
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
	else if (!ft_is_var_in_env(env, "PWD"))
		ft_envadd_back(env, ft_envnew(ft_strdup("PWD"), ft_strdup(pwd)));
	if (!ft_is_var_in_env(env, "OLDPWD"))
		ft_envadd_back(env, ft_envnew(ft_strdup("OLDPWD"), ft_strdup(old_dir)));
	else
		ft_change_env(env, pwd, old_dir);
	free(pwd);
}

static char	*ft_get_new_dir(char *old_dir, char *new_dir, t_env **env)
{
	t_env	*tmp;

	if (!old_dir && !new_dir)
	{
		tmp = *env;
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, "HOME"))
			{
				new_dir = ft_strdup(tmp->value);
				break ;
			}
			tmp = tmp->next;
		}
	}
	else if (!new_dir)
	{
		new_dir = ft_strndup(old_dir, (ft_strlen(old_dir)
					- ft_strlen(ft_strchr(ft_strchr(old_dir, '/') + 6, '/'))));
	}
	else if (!ft_strncmp(new_dir, "~", 1))
		new_dir = ft_gestion_tilde(env, new_dir);
	return (new_dir);
}

int	ft_no_oldir(t_env **env)
{
	t_env	*status;

	status = ft_get_exit_status(env);
	if (status)
		ft_change_exit_status(status, ft_itoa(0));
	else
		ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(0)));
	ft_putstr_fd("minishell: cd: error retrieving current directory: ", 2);
	ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
	ft_putendl_fd("No such file or directory", 2);
	return (0);
}

int	ft_cd_util(char **path, t_env **env)
{
	char	*new_dir;
	char	*old_dir;
	t_env	*status;

	new_dir = ft_strdup(path[1]);
	old_dir = ft_get_old_pwd(env);
	new_dir = ft_get_new_dir(old_dir, new_dir, env);
	ft_old_inexistent(env);
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
