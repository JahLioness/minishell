/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:46:12 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/20 17:03:19 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void close_fd(int *fd, int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
}

int	first_child(t_cmd *cmd)
{
	close(cmd->exec->pipe_fd[0]);
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
	close_fd(cmd->exec->pipe_fd, -1);
	if (cmd->exec->prev_fd != STDIN_FILENO && cmd->exec->prev_fd != -1)
	{
		dup2(cmd->exec->prev_fd, STDIN_FILENO);
		close(cmd->exec->prev_fd);
	}
	if (cmd->exec->redir_out != STDOUT_FILENO && cmd->exec->redir_out != -1)
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

void	process_child(t_ast *curr)
{
	printf("exec->pid == %d\n", curr->token->cmd->exec->pid);
	if (curr->parent->right == curr && (!curr->parent->parent || curr->parent->parent->token->type != T_PIPE))
	{
		printf("LAST CHILD = %s\n", curr->token->cmd->cmd);
		last_child(curr->token->cmd);
	}
	else if (curr->parent->left == curr)
	{
		printf("FIRST = %s\n", curr->token->cmd->cmd);
		first_child(curr->token->cmd);
	}
	else if (curr->parent->right == curr && curr->parent->parent && curr->parent->parent->token->type == T_PIPE)
	{
		printf("MIDDLE CHILD = %s\n", curr->token->cmd->cmd);
		middle_child(curr->token->cmd);
	}
	
}

void	set_prev_fd(t_ast *cmd_ex)
{
	if (cmd_ex->parent->right->token->cmd->exec->prev_fd != -1)
		close(cmd_ex->parent->right->token->cmd->exec->prev_fd);
	if (cmd_ex->parent->left)
	{
		if (cmd_ex->parent->left->token->type == T_CMD)
		{
			cmd_ex->parent->right->token->cmd->exec->prev_fd = cmd_ex->parent->left->token->cmd->exec->pipe_fd[0];
			// close(cmd_ex->parent->left->token->cmd->exec->pipe_fd[0]);
		}
		else if (cmd_ex->parent->left->token->type == T_PIPE)
		{
			cmd_ex->parent->right->token->cmd->exec->prev_fd = cmd_ex->parent->left->right->token->cmd->exec->pipe_fd[0];
			// close(cmd_ex->parent->left->right->token->cmd->exec->pipe_fd[0]);
		}
	}
	if (cmd_ex->parent->left->token->type == T_CMD && cmd_ex->parent->left->token->cmd->exec->pipe_fd[1] != -1)
		close(cmd_ex->parent->left->token->cmd->exec->pipe_fd[1]);
}

//CACA
int ft_exec_pipe(t_ast *cmd_ex, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini *last;
	t_env *e_status;
	t_exec *exec;
	t_cmd *cmd;
	t_redir *tmp;
	char **envp;
	// int tmp_pref_fd = -1;

	tmp = NULL;
	cmd = cmd_ex->token->cmd;
	exec = cmd->exec;
	if (cmd->redir)
		tmp = cmd->redir;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	// gestion des quotes et expand
	if (cmd_ex->token->type == T_CMD && cmd_ex->token->cmd)
	{
		if ((cmd->cmd && cmd->args) || (!cmd->cmd && *cmd->args))
			handle_expand(cmd_ex, last);
		// premier appel de la fonction pour verifier les file
		if (cmd->redir && cmd->redir->type != REDIR_HEREDOC)
		{
			ft_handle_redir_file(cmd);
			reset_fd(exec);
		}
		// cas de redirection pour "cat file" sans sympbole de redirection
		else if (!cmd->redir && cmd->args && !ft_strcmp(cmd->args[0], "cat") && cmd->args[1])
			cat_wt_symbole(cmd, exec);
		// si erreur de file, on execute pas le reste des commandes
		if (exec->error_ex == 1 || (cmd->redir && cmd->redir->type != REDIR_HEREDOC && !cmd->cmd))
		{
			reset_fd(exec);
			e_status = ft_get_exit_status(&last->env);
			ft_change_exit_status(e_status, ft_itoa(1));
		}
		ft_set_var_underscore(cmd->args, &last->env, envp);
		// si pas d'erreur, on execute la commande
		if (!exec->error_ex)
		{
			if (pipe(exec->pipe_fd) == -1)
				return (ft_putendl_fd("minishell: pipe failed", 2), 1);
			exec->pid = fork();
			if (exec->pid < 0)
				return (ft_putendl_fd("minishell: fork failed", 2), 1);
			if (exec->pid == 0)
			{
				process_child(cmd_ex);
				if (ft_is_builtin(cmd->cmd))
				{
					if (tmp && tmp->type != REDIR_HEREDOC)
						builtin_w_redir(tmp, exec);
					exec->status = ft_exec_builtin(cmd_ex->token, &last->env, exec->redir_out);
				}
				exec_command(cmd_ex, granny, mini, prompt);
				close_fd(exec->pipe_fd, exec->prev_fd);
				ft_free_tab(envp);
				exit(EXIT_SUCCESS);
			}
			else
			{
				printf("CURR CMD == %s\n", cmd->cmd);
				if (cmd_ex->parent->left->token->type == T_CMD)
						printf("cmd_ex->parent->left->token->cmd->exec->prev_fd == %d && CMD == %s\n\n", cmd_ex->parent->left->token->cmd->exec->prev_fd, cmd_ex->parent->left->token->cmd->cmd);
				printf("cmd_ex->parent->right->token->cmd->exec->prev_fd == %d && CMD == %s\n\n", cmd_ex->parent->right->token->cmd->exec->prev_fd, cmd_ex->parent->right->token->cmd->cmd);
				if (cmd_ex->parent && cmd_ex->parent->left && cmd_ex->parent->left->right && cmd_ex->parent->left->right->token->type == T_CMD)
				{
					printf("cmd_ex->parent->left->right->token->cmd->exec->pipe_fd[0] == %d\n", cmd_ex->parent->left->right->token->cmd->exec->pipe_fd[0]);
					printf("cmd_ex->parent->left->right->token->cmd->exec->prev_fd == %d\n", cmd_ex->parent->left->right->token->cmd->exec->prev_fd);
				}
				printf("curr pid == %d\n", cmd_ex->token->cmd->exec->pid);
				printf("curr pipe_fd[0] == %d\n", cmd_ex->token->cmd->exec->pipe_fd[0]);
				printf("curr pipe_fd[1] == %d\n", cmd_ex->token->cmd->exec->pipe_fd[1]);
				printf("BEFORE SAVE curr prev_fd == %d\n", cmd_ex->token->cmd->exec->prev_fd);
				set_prev_fd(cmd_ex);
				printf("AFTER SAVE exec->prev_fd == %d\n", exec->prev_fd);
				ft_free_tab(envp);
				waitpid(exec->pid, &exec->status, 0);
				if (cmd_ex->parent && cmd_ex->parent->left->token->type == T_CMD && cmd_ex->parent->left->token->cmd->exec->pipe_fd[0] != -1)
					close(cmd_ex->parent->left->token->cmd->exec->pipe_fd[0]);
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
	}
	close_fd(exec->pipe_fd, exec->prev_fd);
	e_status = ft_get_exit_status(&last->env);
	if (g_sig == SIGQUIT)
	{
		reset_fd(exec);
		ft_change_exit_status(e_status, ft_itoa(131));
		ft_free_tab(envp);
		g_sig = 0;
		return (131);
	}
	return (exec->status);
}

int save_fd(int *fd, int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (fd[1] != -1)
		close(fd[1]);
	prev_fd = fd[0];
	return (prev_fd);
}