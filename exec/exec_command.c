/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:14:38 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/23 13:58:14 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_path(char *path)
{
	struct stat	path_stat;

	if (!path || !*path)
		return (127);
	if (stat(path, &path_stat) != 0)
	{
		msg_error("minishell: ", path, "No such file or directory");
		return (127);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		msg_error("minishell: ", path, "Is a directory");
		return (126);
	}
	return (0);
}

int	check_access(char *path, t_cmd *cmd)
{
	int		exit_status;

	if (!path || !*path)
		return (msg_error("minishell: ", cmd->cmd, "command not found"), 127);
	if (!ft_strcmp(cmd->cmd, "."))
		return (msg_error("minishell .", cmd->cmd,
				"filename argument required"), 2);
	exit_status = check_path(path);
	if (exit_status != 0)
		return (exit_status);
	if (access(path, X_OK) == -1 && errno == EACCES)
		return (msg_error("minishell: ", cmd->cmd, "Permission denied"), 126);
	else if (access(path, F_OK) == -1 && errno == ENOENT)
		return (msg_error("minishell: ", cmd->cmd, "No such file or directory"),
			126);
	return (0);
}

char	*get_full_path(t_cmd *cmd, char **envp)
{
	char	*full_path;

	full_path = NULL;
	if (!ft_strcmp(cmd->cmd, "$_"))
	{
		full_path = ft_tabchr(envp, "_", '=');
		free(cmd->cmd);
		cmd->cmd = ft_strdup(full_path);
		if (!ft_strchr(full_path, '/'))
		{
			free(full_path);
			full_path = ft_get_cmd_path_env(cmd->cmd, envp);
		}
	}
	else if (ft_strchr(cmd->cmd, '/'))
		full_path = ft_strdup(cmd->cmd);
	else
		full_path = ft_get_cmd_path_env(cmd->cmd, envp);
	return (full_path);
}

int	exec_command(t_cmd *cmd, t_exec_utils *e_utils)
{
	char	**envp;
	char	*full_path;
	int		exit_status;
	t_mini	*last;

	full_path = NULL;
	last = ft_minilast(*e_utils->mini);
	envp = ft_get_envp(&last->env);
	if (!cmd->cmd || !*cmd->cmd)
		return (ft_exec_cmd_error(e_utils, envp), 1);
	full_path = get_full_path(cmd, envp);
	exit_status = check_access(full_path, cmd);
	if (exit_status != 0)
	{
		free(full_path);
		ft_exec_cmd_error(e_utils, envp);
		exit(exit_status);
	}
	else if (execve(full_path, cmd->args, envp) == -1)
		return (free(full_path), ft_exec_cmd_error(e_utils, envp), 1);
	return (free(full_path), ft_free_tab(envp), cmd->exec.status);
}
