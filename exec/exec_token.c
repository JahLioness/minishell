/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:41:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/21 15:21:51 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// IL FAUT FAIRE CETTE FOMCTION AUTREMENT
int	ft_exec_multiple_cmd(t_ast *granny, t_ast *current, t_ast *parent,
		t_mini **mini, char *prompt, int status)
{
	int	exit_status;

	// printf("current->token->type = %d\n", current->token->type);
	exit_status = status;
	if (!granny || !current || !mini || !prompt)
		return (-1);
	if (current->parent)
		parent = current->parent;
	if (current->token->type == T_CMD && !current->token->cmd->cmd
		&& current->token->cmd->redir
		&& current->token->cmd->redir->type == REDIR_HEREDOC)
	{
		exit_status = 0;
		return (exit_status);
	}
	else if (current->token->type == T_CMD && parent->token->type == T_PIPE)
	{
		printf("PIPE\n");
		// exit_status = ft_exec_pipe(current, granny, mini, prompt);
	}
	else if (current->token->type == T_CMD && parent->token->type != T_PIPE)
		return (ft_exec_cmd(current, granny, mini, prompt));
	else if (current->token->type == T_PIPE)
	{
		printf("PIPE\n");
		// exit_status = ft_exec_multiple_cmd(granny, current->left, parent,
		//		mini, prompt, exit_status);
		// exit_status = ft_exec_multiple_cmd(granny, current->right, parent,
		//		mini, prompt, exit_status);
	}
	if (current->token->type == T_AND)
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini,
				prompt, exit_status);
		if (exit_status == 0)
			return (ft_exec_multiple_cmd(granny, current->right, parent, mini,
					prompt, exit_status));
	}
	else if (current->token->type == T_OR)
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini,
				prompt, exit_status);
		if (exit_status != 0)
			return (ft_exec_multiple_cmd(granny, current->right, parent, mini,
					prompt, exit_status));
	}
	return (exit_status);
}

t_cmd	*get_heredoc_node(t_mini *last)
{
	t_token	*tmp_token;
	t_redir	*tmp_redir;
	t_cmd	*tmp_cmd;
	t_cmd	*node_heredoc;

	if (!last)
		return (NULL);
	node_heredoc = NULL;
	tmp_token = last->tokens;
	while (tmp_token)
	{
		tmp_cmd = tmp_token->cmd;
		if (tmp_cmd && tmp_cmd->redir)
		{
			tmp_redir = tmp_cmd->redir;
			while (tmp_cmd && tmp_redir)
			{
				if (tmp_redir->type == REDIR_HEREDOC)
				{
					node_heredoc = tmp_cmd;
					break ;
				}
				tmp_redir = tmp_redir->next;
			}
		}
		tmp_token = tmp_token->next;
	}
	return (node_heredoc);
}

void	ft_exec_token(t_mini **mini, char *prompt)
{
	t_mini	*last;
	t_token	*tmp;
	t_token	*last_t;
	t_ast	*root;
	t_cmd	*node_heredoc;

	if (!*mini)
		return ;
	node_heredoc = NULL;
	last = ft_minilast(*mini);
	tmp = last->tokens;
	last_t = ft_tokenlast(tmp);
	if (last->is_heredoc)
	{
		node_heredoc = get_heredoc_node(last);
		handle_heredoc(node_heredoc, mini, prompt);
	}
	root = create_ast(last->tokens, last_t);
	ft_exec_multiple_cmd(root, root, root, mini, prompt, -1);
	// print_ast(root, 0, ' ');
	ft_clear_ast(root);
}

// int ft_exec_multiple_pipe(t_ast *c_left, t_ast *c_right, t_ast *granny,
//		t_mini **mini, char *prompt)
// {
// 	int	fd[2];

// 	if (pipe(fd) == -1)
// 	{
// 		ft_putstr_fd("pipe error\n", 2);
// 		return (-1);
// 	}
// 	pid_t pid = fork();
// 	if (pid == 0)
// 	{
// 		close(fd[READ_END]);
// 		dup2(fd[WRITE_END], STDOUT_FILENO);
// 		close(fd[WRITE_END]);
// 		exec_command(c_left, granny, mini, prompt);
// 		exit(EXIT_SUCCESS);
// 	}
// 	else if (pid < 0)
// 	{
// 		ft_putstr_fd("fork error\n", 2);
// 		return (-1);
// 	}
// 	else
// 	{
// 		pid_t pid2 = fork();
// 		if (pid2 == 0)
// 		{
// 			close(fd[WRITE_END]);
// 			dup2(fd[READ_END], STDIN_FILENO);
// 			close(fd[READ_END]);
// 			exec_command(c_right, granny, mini, prompt);
// 			exit(EXIT_SUCCESS);
// 		}
// 		else if (pid2 < 0)
// 		{
// 			ft_putstr_fd("fork error\n", 2);
// 			return (-1);
// 		}
// 		else
// 		{
// 			close(fd[READ_END]);
// 			close(fd[WRITE_END]);
// 			waitpid(pid, NULL, 0);
// 			waitpid(pid2, NULL, 0);
// 		}
// 	}
// 	return (0);
// }

// t_exec *exec_l;
// t_exec *exec_r;

// exec_l = c_left->token->cmd->exec;
// exec_r = c_right->token->cmd->exec;
// if (pipe(exec_l->pipe_fd) == -1)
// {
// 	ft_putstr_fd("pipe error\n", 2);
// 	return (-1);
// }
// exec_l->pid = fork();
// if (exec_l->pid < 0)
// {
// 	ft_putstr_fd("fork error\n", 2);
// 	return (-1);
// }
// if (exec_l->pid == 0)
// {
// 	if (exec_l->redir_in != -1)
// 	{
// 		dup2(exec_l->redir_in, STDIN_FILENO);
// 		close(exec_l->redir_in);
// 	}
// 	close(exec_l->pipe_fd[0]);
// 	dup2(exec_l->pipe_fd[1], STDOUT_FILENO);
// 	close(exec_l->pipe_fd[1]);
// 	exec_command(c_left, granny, mini, prompt);
// 	exit(EXIT_SUCCESS);
// }
// if (pipe(exec_r->pipe_fd) == -1)
// {
// 	ft_putstr_fd("pipe error\n", 2);
// 	return (-1);
// }
// exec_r->pid = fork();
// if (exec_r->pid < 0)
// {
// 	ft_putstr_fd("fork error\n", 2);
// 	return (-1);
// }
// if (exec_r->pid == 0)
// {
// 	close(exec_l->pipe_fd[1]);
// 	dup2(exec_l->pipe_fd[0], STDIN_FILENO);
// 	close(exec_l->pipe_fd[0]);

// 	if (exec_r->redir_out != -1)
// 	{
// 		dup2(exec_r->redir_out, STDOUT_FILENO);
// 		close(exec_r->redir_out);
// 	}
// 	close(exec_r->pipe_fd[0]);
// 	close(exec_r->pipe_fd[1]);
// 	exec_command(c_right, granny, mini, prompt);
// 	exit(EXIT_SUCCESS);
// }
// exec_r->prev_fd = exec_l->pipe_fd[0];
// close(exec_l->pipe_fd[0]);
// close(exec_l->pipe_fd[1]);
// waitpid(exec_l->pid, &exec_l->status, 0);
// waitpid(exec_r->pid, &exec_r->status, 0);
// return (exec_r->prev_fd);