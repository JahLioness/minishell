/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/30 14:13:07 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_close_pipe(t_cmd *cmd)
{
	if (cmd->exec.prev_fd != -1)
		close(cmd->exec.prev_fd);
	if (cmd->next)
	{
		close(cmd->exec.pipe_fd[1]);
		if (cmd->next->redir && (cmd->next->redir->type == REDIR_HEREDOC || cmd->next->redir->type == REDIR_INPUT))
		{
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

	last = ft_minilast(*e_utils->mini);
	cmd->exec.pid = fork();
	if (cmd->exec.pid < 0)
		return (ft_putendl_fd("minishell: fork failed", 2), 1);
	if (cmd->exec.pid == 0)
	{
		if (cmd->redir)
			handle_redir_dup(&cmd->exec, cmd, last);
		e_utils->envp = ft_free_envp(e_utils);
		process_child(cmd, i, len_cmd);
		reset_fd(&cmd->exec);
		exec_command(cmd, e_utils);
		exit(EXIT_SUCCESS);
	}
	else
	{
		ft_close_pipe(cmd);
		return (0);
	}
}

void	ft_exec_builtins(t_ast *root, t_cmd *cmd, t_exec_utils *e_utils)
{
	t_mini	*last;

	last = ft_minilast(*e_utils->mini);
	if (ft_is_builtin(cmd->cmd))
	{
		handle_builtin(cmd, last, cmd->redir, &cmd->exec);
		ft_close_pipe(cmd);
	}
	else if (!ft_strcmp(cmd->cmd, "exit"))
	{
		e_utils->envp = ft_free_envp(e_utils);
		handle_exit(root, e_utils->mini, e_utils->prompt);
	}
}

int	ft_exec_lst_cmd(t_ast *root, t_exec_utils *e_utils)
{
	int		len_cmd;
	int		i;
	t_mini	*last;
	t_cmd	*cmd;

	i = -1;
	last = ft_minilast(*e_utils->mini);
	cmd = root->token->cmd;
	len_cmd = ft_cmdsize(cmd);
	ft_get_signal_cmd();
	reset_fd(&cmd->exec);
	while (++i < len_cmd)
	{
		printf(" PIPE cmd->cmd = %s\n", cmd->cmd);
		// printf("PIPE cmd->redir->file == %s\n", cmd->redir->file);
		if (pipe(cmd->exec.pipe_fd) < 0)
			return (ft_putendl_fd("minishell: pipe failed", 2), 1);
		ft_exec_builtins(root, cmd, e_utils);
		if (!ft_is_builtin(cmd->cmd) && ft_strcmp(cmd->cmd, "exit"))
			cmd->exec.status = ft_exec_multi_lst_cmd(e_utils, cmd, i, len_cmd);
		reset_fd(&cmd->exec);
		if (cmd->next)
			cmd = cmd->next;
	}
	e_utils->envp = ft_free_envp(e_utils);
	return (ft_waitpid(root->token->cmd, last, len_cmd));
}
