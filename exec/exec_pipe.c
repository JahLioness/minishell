/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:46:12 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/30 19:44:56 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void close_fd(int *fd, int prev_fd)
{
	close(fd[0]);
	close(fd[1]);
	close(prev_fd);
}

int ft_count_cmds(t_token *token)
{
	int count;

	count = 0;
	while (token)
	{
		if (token->type == T_PIPE)
			count++;
		token = token->next;
	}
	return (count + 1);
}

int	first_child(t_cmd *cmd)
{
	dup2(cmd->exec->pipe_fd[1], STDOUT_FILENO);
	close(cmd->exec->pipe_fd[1]);
	if (cmd->exec->redir_in != -1)
	{
		dup2(cmd->exec->redir_in, STDIN_FILENO);
		close(cmd->exec->redir_in);
	}
	return (0);
}

int	last_child(t_cmd *cmd)
{
	printf("prev_fd = %d\n", cmd->exec->prev_fd);
	printf("pipe_fd[0] = %d\n", cmd->exec->pipe_fd[0]);
	printf("pipe_fd[1] = %d\n", cmd->exec->pipe_fd[1]);
	printf("redir_out = %d\n", cmd->exec->redir_out);
	close(cmd->exec->pipe_fd[1]);
	close(cmd->exec->pipe_fd[0]);
	if (cmd->exec->prev_fd != -1)
	{
		dup2(cmd->exec->prev_fd, STDIN_FILENO);
		close(cmd->exec->prev_fd);
	}
	if (cmd->exec->redir_out != -1)
	{
		dup2(cmd->exec->redir_out, STDOUT_FILENO);
		close(cmd->exec->redir_out);
	}
	return (0);
}

int	middle_child(t_cmd *cmd)
{
	close(cmd->exec->pipe_fd[0]);
	dup2(cmd->exec->prev_fd, STDIN_FILENO);
	close(cmd->exec->prev_fd);
	dup2(cmd->exec->pipe_fd[1], STDOUT_FILENO);
	close(cmd->exec->pipe_fd[1]);
	return (0);
}

void	process_child(int num_cmds, int total_cmds, t_cmd *cmd)
{
	printf("num_cmds in processe child = %d\n", num_cmds);
	if (num_cmds == 1)
		last_child(cmd);
	else if (num_cmds == total_cmds)
		first_child(cmd);
	else
		middle_child(cmd);
}

int save_fd(int *fd, int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	close(fd[1]);
	prev_fd = fd[0];
	return (prev_fd);
}

//CACA
int ft_exec_pipe(t_ast *current, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini *last;
	t_env *e_status;
	t_exec *exec;
	t_cmd *cmd;
	t_redir *tmp;
	char **envp;
	int num_cmds;
	int total_cmds;

	tmp = NULL;
	cmd = current->token->cmd;
	exec = cmd->exec;
	if (cmd->redir)
		tmp = cmd->redir;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	num_cmds = ft_count_cmds(last->tokens);
	total_cmds = num_cmds;
	// gestion des quotes et expand
	if (current->token->type == T_CMD && current->token->cmd)
	{
		if ((current->token->cmd->cmd && current->token->cmd->args) || (!current->token->cmd->cmd && *current->token->cmd->args))
			handle_expand(current, last);
		// premier appel de la fonction pour verifier les file
		if (current->token->cmd->redir && current->token->cmd->redir->type != REDIR_HEREDOC)
		{
			ft_handle_redir_file(cmd);
			reset_fd(exec);
		}
		// cas de redirection pour "cat file" sans sympbole de redirection
		else if (!current->token->cmd->redir && current->token->cmd->args && !ft_strcmp(current->token->cmd->args[0], "cat") && current->token->cmd->args[1])
			cat_wt_symbole(current->token->cmd, exec);
		// si erreur de file, on execute pas le reste des commandes
		if (exec->error_ex == 1 || (current->token->cmd->redir && current->token->cmd->redir->type != REDIR_HEREDOC && !current->token->cmd->cmd))
		{
			reset_fd(exec);
			e_status = ft_get_exit_status(&last->env);
			ft_change_exit_status(e_status, ft_itoa(1));
		}
		ft_set_var_underscore(current->token->cmd->args, &last->env, envp);
		// si pas d'erreur, on execute la commande
		if (!exec->error_ex)
		{
			printf("num_cmds = %d\n", num_cmds);
			while (num_cmds-- > -1)
			{
				if (pipe(exec->pipe_fd) == -1)
					return (ft_putendl_fd("minishell: pipe failed", 2), 1);
				if (ft_is_builtin(current->token->cmd->cmd))
				{
					if (tmp && tmp->type != REDIR_HEREDOC)
						builtin_w_redir(tmp, exec);
					if (current->token->cmd->redir)
						exec->status = ft_exec_builtin(current->token, &last->env, exec->redir_out);
					else
						exec->status = ft_exec_builtin(current->token, &last->env, exec->pipe_fd[1]);
				}
				exec->pid = fork();
				if (exec->pid < 0)
					return (ft_putendl_fd("minishell: fork failed", 2), 1);
				if (exec->pid == 0)
				{
					process_child(num_cmds, total_cmds, cmd);
					exec_command(current, granny, mini, prompt); 
					ft_free_tab(envp);
					exit(EXIT_SUCCESS);
				}
				else
				{
					exec->prev_fd = save_fd(exec->pipe_fd, exec->prev_fd);
					ft_free_tab(envp);
					// reset_fd(exec);
					close_fd(exec->pipe_fd, exec->prev_fd);
					waitpid(exec->pid, &exec->status, 0);
					if (WIFEXITED(exec->status))
					{
						e_status = ft_get_exit_status(&last->env);
						if (g_sig == SIGINT)
						{
							kill(cmd->exec->pid, SIGKILL);
							ft_change_exit_status(e_status, ft_itoa(130));
							g_sig = 0;
							return (130);
						}
						return (set_e_status(cmd->exec->status, last));
					}
				}
			}
			// exec->status = ft_exec_pipe(current->parent->right, granny, mini, prompt);
		}
	}
	e_status = ft_get_exit_status(&last->env);
	if (g_sig == SIGQUIT)
	{
		reset_fd(exec);
		ft_change_exit_status(e_status, ft_itoa(131));
		ft_free_tab(envp);
		g_sig = 0;
		return (131);
	}
	if (envp)
		ft_free_tab(envp);
	return (exec->status);
}

void	create_pipe(t_exec *exec)
{
	if (pipe(exec->pipe_fd) == -1)
	{
		ft_putendl_fd("minishell: pipe failed", 2);
		exit(EXIT_FAILURE);
	}
}

int	count_cmd(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (token->type == T_CMD)
			count++;
		token = token->next;
	}
	return (count);
}

// void gestion_pipe(t_ast *root, t_ast *granny, t_mini **mini, char *prompt, int input, int output)
// {
// 	pid_t pid;

// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		ft_putendl_fd("minishell: fork failed", 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid < 0)
// 	{
// 		ft_putendl_fd("minishell: fork failed", 2);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		if (input != -1)
// 		{
// 			dup2(input, STDIN_FILENO);
// 			close(input);
// 		}
// 		if (output != -1)
// 		{
// 			dup2(output, STDOUT_FILENO);
// 			close(output);
// 		}
// 	}
// 	exec_command(root, granny, mini, prompt);
// }

void read_pipe(int fd)
{
	char buffer[1024];
	ssize_t bytes_read;
	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
		write(STDOUT_FILENO, buffer, bytes_read);
	printf("t la\n");	
	close(fd);
}

// void	ft_exec_pipe(t_ast *root, t_ast *granny, t_mini **mini, char *prompt, int input_fd, int num_cmds)
// {
// 	if (!root || num_cmds == 0)
// 	{
// 		return;
// 	}

// 	int pipefd[2];
// 	t_cmd *cmd = root->token->cmd;
// 	t_exec *exec = cmd->exec;

// 	if (num_cmds > 0)
// 	{
// 		if (pipe(pipefd) == -1)
// 		{
// 			perror("pipe");
// 			exit(EXIT_FAILURE);
// 		}
// 	}

// 	exec->pid = fork();
// 	if (exec->pid < 0)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (exec->pid == 0)
// 	{
// 		if (input_fd != -1)
// 		{
// 			dup2(input_fd, STDIN_FILENO);
// 			close(input_fd);
// 		}
// 		read_pipe(input_fd);
// 		if (num_cmds > 0)
// 		{
// 			close(pipefd[0]);
// 			dup2(pipefd[1], STDOUT_FILENO);
// 			close(pipefd[1]);
// 		}
// 		exec_command(root, granny, mini, prompt);
// 		exit(EXIT_SUCCESS);
// 	}
// 	else
// 	{
// 		if (input_fd != -1)
// 		{
// 			close(input_fd);
// 		}
// 		if (num_cmds > 1)
// 		{
// 			close(pipefd[1]);
// 			ft_exec_pipe(root->right, granny, mini, prompt, pipefd[0], num_cmds);
// 			close(pipefd[0]);
// 		}
// 		waitpid(exec->pid, &exec->status, 0);
// 		if (WIFEXITED(exec->status))
// 		{
// 			t_env *e_status = ft_get_exit_status(&((*mini)->env));
// 			if (g_sig == SIGINT)
// 			{
// 				kill(exec->pid, SIGKILL);
// 				ft_change_exit_status(e_status, "130");
// 				g_sig = 0;
// 			}
// 			else
// 			{
// 				set_e_status(exec->status, ft_minilast(*mini));
// 			}
// 		}
// 	}
// }
	