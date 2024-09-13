/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:34:53 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/12 13:48:36 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_builtin(t_cmd *cmd, t_env **env, int fd)
{
	t_env	*status;
	int		exit_status;

	exit_status = 0;
	if (!ft_strcmp(cmd->cmd, "echo"))
		exit_status = ft_echo(fd, ft_get_args_echo(cmd->args, env),
				ft_get_flag_echo(cmd->args));
	else if (!ft_strcmp(cmd->cmd, "cd"))
		exit_status = ft_cd(cmd->args, env);
	else if (!ft_strcmp(cmd->cmd, "pwd"))
		exit_status = ft_pwd(fd, env);
	else if (!ft_strcmp(cmd->cmd, "export"))
		exit_status = ft_exec_export(cmd, env, fd);
	else if (!ft_strcmp(cmd->cmd, "unset"))
		exit_status = ft_exec_unset(cmd, env, fd);
	else if (!ft_strcmp(cmd->cmd, "env"))
		exit_status = ft_print_env(env, fd);
	status = ft_get_exit_status(env);
	if (!status)
		ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(exit_status)));
	else
		ft_change_exit_status(status, ft_itoa(exit_status));
	return (exit_status);
}

int	ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}
