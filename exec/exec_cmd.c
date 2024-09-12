/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/12 12:07:57 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_close_pipe(t_cmd *cmd)
{
	if (cmd->exec.prev_fd != -1 && cmd->exec.prev_fd != STDIN_FILENO)
		close(cmd->exec.prev_fd);
	if (cmd->next)
	{
		if (cmd->exec.pipe_fd[1] != -1 && cmd->exec.pipe_fd[1] != STDOUT_FILENO)
			close(cmd->exec.pipe_fd[1]);
		if (cmd->next->redir && (cmd->next->redir->type == REDIR_HEREDOC
				|| cmd->next->redir->type == REDIR_INPUT))
		{
			if (cmd->exec.pipe_fd[0] != -1
				&& cmd->exec.pipe_fd[0] != STDIN_FILENO)
				close(cmd->exec.pipe_fd[0]);
			cmd->next->exec.prev_fd = cmd->next->exec.redir_in;
		}
		else
			cmd->next->exec.prev_fd = cmd->exec.pipe_fd[0];
	}
}

int	ft_exec_multi_lst_cmd(t_exec_utils *e_utils, t_cmd *cmd, int i, int len_cmd)
{
	t_mini	*last;
	int		status;

	last = ft_minilast(*e_utils->mini);
	cmd->exec.pid = fork();
	if (cmd->exec.pid < 0)
		return (ft_putendl_fd("minishell: fork failed", 2), 1);
	if (cmd->exec.pid == 0)
	{
		process_child(cmd, i, len_cmd);
		if (ft_is_builtin(cmd->cmd) && ft_strcmp(cmd->cmd, "exit"))
		{
			handle_builtin(cmd, last, cmd->redir, &cmd->exec);
			status = cmd->exec.status;
			reset_fd(&cmd->exec);
			ft_close_pipe(cmd);
			ft_exec_cmd_error(e_utils, e_utils->envp);
			exit(status);
		}
		else if (!ft_strcmp(cmd->cmd, "exit"))
		{
			ft_free_envp(e_utils);
			handle_exit(e_utils, cmd);
			exit(EXIT_SUCCESS);
		}
		if (cmd->redir)
			handle_redir_dup(&cmd->exec, cmd, last);
		ft_free_envp(e_utils);
		reset_fd(&cmd->exec);
		ft_close_pipe(cmd);
		exec_command(cmd, e_utils);
		exit(EXIT_FAILURE);
	}
	else
	{
		ft_close_pipe(cmd);
		return (cmd->exec.status);
	}
}

void	ft_exec_builtins(t_ast *root, t_cmd *cmd, t_exec_utils *e_utils)
{
	t_mini	*last;

	(void)root;
	last = ft_minilast(*e_utils->mini);
	if (ft_is_builtin(cmd->cmd) && ft_strcmp(cmd->cmd, "exit"))
	{
		handle_builtin(cmd, last, cmd->redir, &cmd->exec);
		ft_close_pipe(cmd);
	}
	else if (!ft_strcmp(cmd->cmd, "exit"))
	{
		ft_free_envp(e_utils);
		handle_exit(e_utils, cmd);
	}
}

// static void	ft_check_exec_error(t_cmd *cmd, t_ast *root, t_exec_utils *e_utils,
// 		int i)
// {
// 	if (cmd->exec.error_ex)
// 		unlink_files(cmd);
// 	else if (!cmd->exec.error_ex)
// 	{
// 		if (ft_is_builtin(cmd->cmd))
// 			ft_exec_builtins(root, cmd, e_utils);
// 		if (!ft_is_builtin(cmd->cmd) && ft_strcmp(cmd->cmd, "exit"))
// 			cmd->exec.status = ft_exec_multi_lst_cmd(e_utils, cmd, i,
// 					e_utils->len_cmd);
// 		reset_fd(&cmd->exec);
// 	}
// }

int	ft_exec_lst_cmd(t_ast *root, t_exec_utils *e_utils)
{
	int		i;
	t_mini	*last;
	t_cmd	*cmd;

	i = -1;
	last = ft_minilast(*e_utils->mini);
	cmd = root->token->cmd;
	e_utils->len_cmd = ft_cmdsize(cmd);
	ft_get_signal_cmd();
	reset_fd(&cmd->exec);
	e_utils->current = root;
	while (++i < e_utils->len_cmd)
	{
		if ((cmd->cmd && cmd->args) || (!cmd->cmd && *cmd->args))
			handle_expand(cmd, last);
		if (cmd->redir)
			handle_redir(cmd, e_utils->mini);
		ft_set_var_underscore(cmd->args, &last->env, e_utils->envp);
		if (pipe(cmd->exec.pipe_fd) < 0)
			return (ft_putendl_fd("minishell: pipe failed", 2), 1);
		if (cmd->exec.error_ex)
			unlink_files(cmd);
		else if (!cmd->exec.error_ex)
			ft_exec_multi_lst_cmd(e_utils, cmd, i, e_utils->len_cmd);
		// ft_check_exec_error(cmd, root, e_utils, i);
		if (cmd->next)
			cmd = cmd->next;
	}
	return (ft_free_envp(e_utils), ft_waitpid(root->token->cmd, last,
			e_utils->len_cmd));
}
