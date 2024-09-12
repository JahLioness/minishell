/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:09:22 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/12 10:54:50 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_export_utils	ft_init_export_utils(void)
{
	t_export_utils	utils;

	utils.key = NULL;
	utils.value = NULL;
	utils.op = NULL;
	return (utils);
}

static t_export_utils	ft_new_export_utils(char *key, char *value, char *op)
{
	t_export_utils	utils;

	utils = ft_init_export_utils();
	utils.key = key;
	utils.value = value;
	utils.op = op;
	return (utils);
}

int	ft_check_var_exist(t_env **env, char **args, int i, int fd)
{
	t_env			*tmp;
	int				exist;
	t_export_utils	utils;

	exist = 0;
	tmp = *env;
	utils = ft_new_export_utils(args[i], NULL, NULL);
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, args[i]))
		{
			exist = 1;
			break ;
		}
		tmp = tmp->next;
	}
	if (!exist)
		return (ft_export(&utils, env, fd));
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(args[i + 1], 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

int	ft_exec_no_var(char *arg, t_env **env, int fd)
{
	t_export_utils	utils;
	int				status;

	utils = ft_init_export_utils();
	status = 1;
	ft_exec_export_utils(arg, &utils);
	if (utils.key)
		status = ft_export(&utils, env, fd);
	if (utils.op)
		free(utils.op);
	if (utils.key)
		free(utils.key);
	return (status);
}

int	ft_exec_export(t_cmd *cmd, t_env **env, int fd)
{
	char			**args;
	int				i;
	int				exit_status;
	t_export_utils	utils;

	i = 1;
	utils = ft_init_export_utils();
	args = cmd->args;
	if (!args[i])
		return (ft_export(&utils, env, fd));
	while (args[i])
	{
		if (args[i + 1] && (args[i + 1][0] == '+' || args[i + 1][0] == '='))
			exit_status = ft_check_var_exist(env, args, i, fd);
		else
			exit_status = ft_exec_no_var(args[i], env, fd);
		i++;
	}
	return (exit_status);
}
