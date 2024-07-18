/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/18 19:28:46 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	process_parent_heredoc(t_cmd *cmd, t_mini *last, struct termios orig_termios)
{
	t_env	*e_status;

	if (cmd->exec->redir_fd != -1 && cmd->exec->redir_fd != STDOUT_FILENO)
		close(cmd->exec->redir_fd);
	waitpid(cmd->exec->pid, &cmd->exec->status, 0);
	if (WIFEXITED(cmd->exec->status))
	{
		e_status = ft_get_exit_status(&last->env);
		if (g_sig == SIGINT)
		{
			unlink(cmd->redir->file_heredoc);
			kill(cmd->exec->pid, SIGKILL);
			cmd->exec->error_ex = 1;
			ft_putendl_fd("^C", 1);
			tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
			if (e_status)
				ft_change_exit_status(e_status, ft_itoa(130));
			g_sig = 0;
			return (130);
		}
		if (e_status)
			ft_change_exit_status(e_status,
				ft_itoa(WEXITSTATUS(cmd->exec->status)));
		return (WEXITSTATUS(cmd->exec->status));
	}
	return (cmd->exec->status);
}

void	ft_get_heredoc(t_cmd *cmd, t_mini *last, t_redir *current_redir)
{
	int		urandom_fd;
	char	*line;
	int		i;

	line = NULL;
	i = 0;
	urandom_fd = open(current_redir->file_heredoc, O_CREAT | O_WRONLY | O_TRUNC,
			0644);
	if (urandom_fd < 0)
		return ;
	while (1)
	{
		ft_get_signal_heredoc();
		i++;
		line = readline("> ");
		if (ft_strchr(line, '$'))
			line = handle_expand_heredoc(cmd, last, line);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: ", 2);
			ft_putstr_fd("here_document at line ", 2);
			ft_putnbr_unsigned(i);
			ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(cmd->redir->file, 2);
			ft_putendl_fd("')", 2);
			break ;
		}
		else if (!ft_strcmp(line, current_redir->file))
			break ;
		else
		{
			ft_putstr_fd(line, urandom_fd);
			ft_putstr_fd("\n", urandom_fd);
			free(line);
		}
	}
	close(urandom_fd);
	free(line);
}

int	handle_heredoc(t_cmd *node_heredoc, t_mini **mini, char *prompt)
{
	t_redir			*redir;
	t_cmd			*cmd;
	t_mini			*last;
	// t_env			*e_status;
	struct termios	orig_termios;

	last = ft_minilast(*mini);
	if (node_heredoc == NULL)
		cmd = last->tokens->cmd;
	else
		cmd = node_heredoc;
	redir = cmd->redir;
	ft_bzero(&orig_termios, sizeof(orig_termios));
	tcgetattr(STDIN_FILENO, &orig_termios);
	redir = cmd->redir;
	generate_heredoc_file(redir);
	cmd->exec->pid = fork();
	if (cmd->exec->pid < 0)
		return (ft_putendl_fd("minishell: fork failed", 2), 1);
	if (cmd->exec->pid == 0)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->file_heredoc)
				ft_get_heredoc(cmd, last, redir);
			redir = redir->next;
		}
		if (cmd->exec->redir_fd != -1)
		{
			close(cmd->exec->redir_fd);
			cmd->exec->redir_fd = -1;
		}
		cmd->exec->redir_fd = STDOUT_FILENO;
		ft_clear_lst(mini);
		free(prompt);
		exit(EXIT_SUCCESS);
	}
	else
		return (process_parent_heredoc(cmd, last, orig_termios));
	return (cmd->exec->status);
}
