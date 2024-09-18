/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:59:24 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/18 11:41:52 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_child_heredoc(t_cmd *cmd, t_mini *last, t_mini **mini,
		char *prompt)
{
	t_redir	*redir;

	redir = cmd->redir;
	while (redir)
	{
		if (redir->file_heredoc)
			ft_get_heredoc(cmd, last, redir);
		redir = redir->next;
	}
	reset_fd(&cmd->exec);
	cmd->exec.redir_out = STDOUT_FILENO;
	if ((cmd->cmd && cmd->args) || (!cmd->cmd && *cmd->args))
		handle_expand(cmd, last);
	ft_clear_lst(mini);
	free(prompt);
	exit(EXIT_SUCCESS);
}

int	process_parent_heredoc(t_cmd *cmd, t_mini *last,
		struct termios orig_termios)
{
	t_env	*e_status;

	reset_fd(&cmd->exec);
	waitpid(cmd->exec.pid, &cmd->exec.status, 0);
	if (WIFEXITED(cmd->exec.status))
	{
		e_status = ft_get_exit_status(&last->env);
		if (g_sig == SIGINT)
		{
			kill(cmd->exec.pid, SIGKILL);
			cmd->exec.error_ex = 1;
			ft_putendl_fd("^C", 1);
			tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
			if (e_status)
				ft_change_exit_status(e_status, ft_itoa(130));
			g_sig = 0;
			return (130);
		}
		return (set_e_status(cmd->exec.status, last));
	}
	return (cmd->exec.status);
}

int	handle_heredoc(t_cmd *node_heredoc, t_mini **mini, char *prompt)
{
	t_redir			*redir;
	t_cmd			*cmd;
	t_mini			*last;
	struct termios	orig_termios;

	last = ft_minilast(*mini);
	if (node_heredoc == NULL)
		cmd = last->tokens->cmd;
	else
		cmd = node_heredoc;
	ft_bzero(&orig_termios, sizeof(orig_termios));
	tcgetattr(STDIN_FILENO, &orig_termios);
	redir = cmd->redir;
	generate_heredoc_file(redir);
	cmd->exec.pid = fork();
	if (cmd->exec.pid < 0)
		return (ft_putendl_fd("minishell: fork failed", 2), 1);
	if (cmd->exec.pid == 0)
		process_child_heredoc(cmd, last, mini, prompt);
	else
		return (process_parent_heredoc(cmd, last, orig_termios));
	return (cmd->exec.status);
}
