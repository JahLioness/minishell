/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:37:11 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/21 15:18:57 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_tabchr(char **tab, char *str, char c)
{
	int		i;
	char	**tmp;

	i = 0;
	if (!tab || !*tab)
		return (NULL);
	while (tab[i])
	{
		tmp = ft_split(tab[i], c);
		if (!tmp)
			return (NULL);
		if (!ft_strcmp(tmp[0], str))
			return (ft_free_tab(tmp), ft_strdup(ft_strrchr(tab[i], '=') + 1));
		ft_free_tab(tmp);
		i++;
	}
	return (NULL);
}

char	*ft_get_cmd_path(char *cmd, char *cmd_path)
{
	int		i;
	char	**cmd_path_tab;
	char	*cmd_path_tmp;
	char	*cmd_path_tmp2;

	i = 0;
	cmd_path_tab = ft_split(cmd_path, ':');
	if (!cmd_path_tab)
		return (free(cmd_path), NULL);
	cmd_path_tmp2 = NULL;
	while (cmd_path_tab[i])
	{
		cmd_path_tmp = ft_strjoin(cmd_path_tab[i], "/");
		cmd_path_tmp2 = ft_strjoin_free(cmd_path_tmp, cmd);
		if (!cmd_path_tmp2)
			return (free(cmd_path_tmp), NULL);
		if (access(cmd_path_tmp2, F_OK) == 0)
			break ;
		free(cmd_path_tmp2);
		cmd_path_tmp2 = NULL;
		i++;
	}
	ft_free_tab(cmd_path_tab);
	free(cmd_path);
	return (cmd_path_tmp2);
}

char	*ft_get_cmd_path_env(char *cmd, char **env)
{
	char	*cmd_path;
	char	*cmd_w_path;

	cmd_path = ft_tabchr(env, "PATH", '=');
	if (!cmd_path)
		return (NULL);
	cmd_w_path = ft_get_cmd_path(cmd, cmd_path);
	return (cmd_w_path);
}

char	**ft_get_envp(t_env **env)
{
	t_env	*tmp;
	char	**envp;
	int		i;

	tmp = *env;
	envp = ft_calloc(ft_envsize(*env) + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	i = 0;
	while (tmp)
	{
		envp[i] = ft_strjoin(tmp->key, "=");
		if (tmp->value)
			envp[i] = ft_strjoin_free(envp[i], tmp->value);
		if (!envp[i])
		{
			ft_free_split(i, envp);
			return (NULL);
		}
		tmp = tmp->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

int	set_e_status(int status, t_mini *last)
{
	t_env	*e_status;

	e_status = ft_get_exit_status(&last->env);
	if (e_status)
		ft_change_exit_status(e_status,
			ft_itoa(WEXITSTATUS(status)));
	else
		ft_envadd_back(&last->env,
			ft_envnew(ft_strdup("?"),
				ft_itoa(WEXITSTATUS(status))));
	return (WEXITSTATUS(status));
}
