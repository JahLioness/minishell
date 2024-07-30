/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 21:46:12 by andjenna          #+#    #+#             */
/*   Updated: 2024/07/26 02:54:38 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//CACA
int ft_exec_pipe(t_ast *current, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini *last;
	t_env *e_status;
	t_exec *exec;
	t_cmd *cmd;
	t_redir *tmp;
	char **envp;

	tmp = NULL;
	cmd = current->token->cmd;
	exec = cmd->exec;
	if (cmd->redir)
		tmp = cmd->redir;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
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
			if (ft_is_builtin(current->token->cmd->cmd))
			{
				if (tmp && tmp->type != REDIR_HEREDOC)
					builtin_w_redir(tmp, exec);
				else
					exec->redir_out = STDOUT_FILENO;
				exec->status = ft_exec_builtin(current->token, &last->env,
											   exec->redir_out);
				reset_fd(exec);
				ft_free_tab(envp);
				envp = NULL;
			}
			else if (!ft_strcmp(current->token->cmd->cmd, "exit"))
			{
				exec->status = ft_exit(current, mini, prompt, envp);
				e_status = ft_get_exit_status(&last->env);
				ft_change_exit_status(e_status, ft_itoa(exec->status));
				ft_free_tab(envp);
				envp = NULL;
			}
			else
			{
				ft_get_signal_cmd();
				reset_fd(exec);
				exec->pid = fork();
				if (pipe(exec->pipe_fd) == -1)
					return (ft_putendl_fd("minishell: pipe failed", 2), 1);
				if (exec->pid < 0)
					return (ft_putendl_fd("minishell: fork failed", 2), 1);
				if (exec->pid == 0)
				{
					// deuxieme appel de la fonction pour verifier les file et here_doc
					if (current->token->cmd->redir)
						ft_handle_redir_file(cmd);
					if (current->token->cmd->redir && ((exec->redir_in != -1 && exec->redir_in != STDOUT_FILENO) || (exec->redir_out != -1 && exec->redir_out != STDOUT_FILENO)))
					{
						close(exec->pipe_fd[0]);
						dup2(exec->pipe_fd[1], STDOUT_FILENO);
						close(exec->pipe_fd[1]);
						dup2(exec->pipe_fd[0], STDIN_FILENO);
						close(exec->pipe_fd[0]);
						dup2(exec->redir_in, STDIN_FILENO);
						dup2(exec->redir_out, STDOUT_FILENO);
					}
					// cas de redirection pour "cat file" sans sympbole de redirection
					else if (!current->token->cmd->redir && !ft_strcmp(current->token->cmd->args[0], "cat") && current->token->cmd->args[1])
					{
						cat_wt_symbole(current->token->cmd, exec);
						close(exec->pipe_fd[0]);
						dup2(exec->pipe_fd[1], STDOUT_FILENO);
						close(exec->pipe_fd[1]);
						dup2(exec->redir_in, STDIN_FILENO);
						dup2(exec->redir_out, STDOUT_FILENO);
						// reset_fd(exec->redir_fd);
					}
					reset_fd(exec);
					ft_free_tab(envp);
					// execution de la commande
					exec_command(current, granny, mini, prompt);
					exit(EXIT_SUCCESS);
				}
				else
				{
					ft_free_tab(envp);
					reset_fd(exec);
					waitpid(exec->pid, &exec->status, 0);
					if (WIFEXITED(exec->status))
					{
						e_status = ft_get_exit_status(&last->env);
						if (g_sig == SIGINT)
						{
							kill(cmd->exec->pid, SIGKILL);
							ft_change_exit_status(e_status, ft_itoa(130));
							g_sig = 0;
							ft_free_tab(envp);
							return (130);
						}
						return (set_e_status(cmd->exec->status, last));
					}
				}
			}
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
