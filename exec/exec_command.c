/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:14:38 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/23 20:51:55 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//faut que tu me montre ou on set $_
// si aucun commande n'a ete lancee avant
// afficher " ] : command not found"
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
	char	*tmp_path;
	int		exit_status;

	if (!path || !*path)
		return (msg_error("minishell: ", cmd->cmd, "command not found"), 127);
	tmp_path = path;
	exit_status = 0;
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

char	*get_full_path(t_ast *root, char **envp)
{
	char	*full_path;

	full_path = NULL;
	if (!ft_strcmp(root->token->cmd->cmd, "$_"))
		full_path = ft_tabchr(envp, "_", '=');
	else if (ft_strchr(root->token->cmd->cmd, '/'))
		full_path = ft_strdup(root->token->cmd->cmd);
	else
		full_path = ft_get_cmd_path_env(root->token->cmd->cmd, envp);
	return (full_path);
}

int	exec_command(t_ast *root, t_ast *granny, t_mini **mini, char *prompt)
{
	char	**envp;
	char	*full_path;
	int		exit_status;
	t_mini	*last;

	exit_status = 0;
	full_path = NULL;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	if (root->token->cmd->redir)
		unlink_files(root->token->cmd->redir);
	if (!root->token->cmd->cmd || !*root->token->cmd->cmd)
		return (ft_exec_cmd_error(granny, mini, envp, prompt), 1);
	full_path = get_full_path(root, envp);
	exit_status = check_access(full_path, root->token->cmd);
	if (exit_status != 0)
	{
		free(full_path);
		ft_exec_cmd_error(granny, mini, envp, prompt);
		exit(exit_status);
	}
	else if (execve(full_path, root->token->cmd->args, envp) == -1)
		return (free(full_path), ft_exec_cmd_error(granny, mini, envp, prompt),
			1);
	return (free(full_path), ft_free_tab(envp), 0);
}

// if (!ft_strcmp(root->token->cmd->cmd, "."))
// 	return ((msg_error("minishell .", root->token->cmd->cmd,
// 				"filename argument required"),
// 				free(full_path),
// 				exit_free(granny, mini, envp, prompt)));
// else if (!ft_strcmp(root->token->cmd->cmd, "$_"))
// 	full_path = ft_tabchr(envp, "_", '=');
// else if (ft_strchr(root->token->cmd->cmd, '/'))
// 	full_path = handle_backslash(root->token->cmd, last);
// else
// 	full_path = handle_cmd(root->token->cmd, tmp_envp, last);