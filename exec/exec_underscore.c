/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_underscore.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:36:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/23 20:55:22 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_set_var_underscore(char **args, t_env **env, char **envp)
{
	t_export_utils	utils;
	int				len;
	char			*arg;

	if (!args || !*args)
		return ;
	len = ft_tab_len(args);
	if (len == 1 && !ft_strcmp(args[0], "$_"))
		return ;
	if (ft_strchr(args[len - 1], '/'))
		arg = ft_strdup(args[len - 1]);
	else
		arg = ft_get_cmd_path_env(args[len - 1], envp);
	if (!arg)
	{
		arg = ft_strdup(args[len - 1]);
		if (!arg)
			return ;
	}
	utils.key = "_";
	utils.op = "=";
	utils.value = ft_strdup(arg);
	free(arg);
	ft_export(&utils, env, 1);
}
