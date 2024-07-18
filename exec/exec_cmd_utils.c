/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:37:11 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/16 17:08:12 by ede-cola         ###   ########.fr       */
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

int	ft_exec_cmd_error(t_ast *root, t_mini **mini, char **envp, char *prompt)
{
	t_mini	*last;
	char	*underscore;

	underscore = ft_tabchr(envp, "_", '=');
	last = ft_minilast(*mini);
	ft_putstr_fd("minishell: ", 2);
	if (!ft_strcmp(last->tokens->cmd->cmd, "$_"))
		ft_putstr_fd(underscore, 2);
	else
		ft_putstr_fd(last->tokens->cmd->cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	// free(root->token->cmd->exec);
	if (envp)
		ft_free_tab(envp);
	if (root)
		ft_clear_ast(root);
	ft_clear_lst(mini);
	free(prompt);
	free(underscore);
	exit(127);
}
