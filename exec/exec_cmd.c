/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/30 15:35:26 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_builtin(t_token *token, t_env **env, int fd)
{
	t_env	*status;
	int		exit_status;

	exit_status = 0;
	if (!ft_strcmp(token->cmd->cmd, "echo"))
		exit_status = ft_echo(fd, ft_get_args_echo(token->cmd->args, env),
				ft_get_flag_echo(token->cmd->args));
	else if (!ft_strcmp(token->cmd->cmd, "cd"))
		exit_status = ft_cd(token->cmd->args, env);
	else if (!ft_strcmp(token->cmd->cmd, "pwd"))
		exit_status = ft_pwd(fd, env);
	else if (!ft_strcmp(token->cmd->cmd, "export"))
		exit_status = ft_exec_export(token, env, fd);
	else if (!ft_strcmp(token->cmd->cmd, "unset"))
		exit_status = ft_exec_unset(token->cmd, env);
	else if (!ft_strcmp(token->cmd->cmd, "env"))
		exit_status = ft_print_env(env, fd);
	status = ft_get_exit_status(env);
	ft_change_exit_status(status, ft_itoa(exit_status));
	return (exit_status);
}

int	ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

void	ft_handle_empty_first_arg(t_ast *root)
{
	int	i;

	i = 0;
	if (root->token->cmd->cmd)
		free(root->token->cmd->cmd);
	root->token->cmd->cmd = ft_strdup(root->token->cmd->args[1]);
	i = 0;
	while (root->token->cmd->args[i + 1])
	{
		free(root->token->cmd->args[i]);
		root->token->cmd->args[i] = ft_strdup(root->token->cmd->args[i + 1]);
		i++;
	}
	free(root->token->cmd->args[i]);
	root->token->cmd->args[i] = NULL;
}

void	handle_expand(t_ast *root, t_mini *last)
{
	int	i;

	i = 0;
	while (root->token->cmd->args[i])
	{
		root->token->cmd->args[i] = ft_verif_arg(root->token->cmd->args,
													&last->env,
													root->token->cmd,
													i);
		i++;
	}
	root->token->cmd->args = ft_trim_quote_args(root->token->cmd->args);
	if (root->token->cmd->args && root->token->cmd->args[0])
	{
		if (root->token->cmd->cmd)
			free(root->token->cmd->cmd);
		root->token->cmd->cmd = ft_strdup(root->token->cmd->args[0]);
	}
	if (root->token->cmd->args && !*root->token->cmd->args[0]
		&& root->token->cmd->args[1])
		ft_handle_empty_first_arg(root);
}

int	ft_exec_cmd(t_ast *root, t_ast *granny, t_mini **mini, char *prompt)
{
	t_mini	*last;
	t_env	*e_status;
	t_exec	*exec;
	t_cmd	*cmd;
	t_redir	*tmp;
	char	**envp;

	tmp = NULL;
	cmd = root->token->cmd;
	exec = cmd->exec;
	if (cmd->redir)
		tmp = cmd->redir;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	// gestion des quotes et expand
	if (root->token->type == T_CMD && root->token->cmd)
	{
		if ((root->token->cmd->cmd && root->token->cmd->args)
			|| (!root->token->cmd->cmd && *root->token->cmd->args))
			handle_expand(root, last);
		// premier appel de la fonction pour verifier les file
		if (root->token->cmd->redir
			&& root->token->cmd->redir->type != REDIR_HEREDOC)
		{
			ft_handle_redir_file(cmd);
			reset_fd(exec);
		}
		// cas de redirection pour "cat file" sans sympbole de redirection
		else if (!root->token->cmd->redir
				&& root->token->cmd->args && !ft_strcmp(root->token->cmd->args[0], "cat")
				&& root->token->cmd->args[1])
			cat_wt_symbole(root->token->cmd, exec);
		// si erreur de file, on execute pas le reste des commandes
		if (exec->error_ex == 1 || (root->token->cmd->redir
				&& root->token->cmd->redir->type != REDIR_HEREDOC
				&& !root->token->cmd->cmd))
		{
			reset_fd(exec);
			e_status = ft_get_exit_status(&last->env);
			ft_change_exit_status(e_status, ft_itoa(1));
		}
		ft_set_var_underscore(root->token->cmd->args, &last->env, envp);
		// si pas d'erreur, on execute la commande
		if (!exec->error_ex)
		{
			if (ft_is_builtin(root->token->cmd->cmd))
			{
				if (tmp && tmp->type != REDIR_HEREDOC)
					builtin_w_redir(tmp, exec);
				else
					exec->redir_out = STDOUT_FILENO;
				exec->status = ft_exec_builtin(root->token, &last->env,
						exec->redir_out);
				reset_fd(exec);
				ft_free_tab(envp);
				envp = NULL;
			}
			else if (!ft_strcmp(root->token->cmd->cmd, "exit"))
			{
				exec->status = ft_exit(root, mini, prompt, envp);
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
				if (exec->pid < 0)
					return (ft_putendl_fd("minishell: fork failed", 2), 1);
				if (exec->pid == 0)
				{
					// deuxieme appel de la fonction pour verifier les file et here_doc
					if (root->token->cmd->redir)
						ft_handle_redir_file(cmd);
					if (root->token->cmd->redir && ((exec->redir_in != -1
							&& exec->redir_in != STDOUT_FILENO) || (exec->redir_out != -1
							&& exec->redir_out != STDOUT_FILENO)))
					{
						dup2(exec->redir_in, STDIN_FILENO);
						dup2(exec->redir_out, STDOUT_FILENO);
					}
					// cas de redirection pour "cat file" sans sympbole de redirection
					else if (!root->token->cmd->redir
							&& !ft_strcmp(root->token->cmd->args[0], "cat")
							&& root->token->cmd->args[1])
					{
						cat_wt_symbole(root->token->cmd, exec);
						dup2(exec->redir_in, STDIN_FILENO);
						dup2(exec->redir_out, STDOUT_FILENO);
						// reset_fd(exec->redir_fd);
					}
					reset_fd(exec);
					ft_free_tab(envp);
					// execution de la commande
					exec_command(root, granny, mini, prompt);
					exit(EXIT_SUCCESS);
				}
				else
				{
					if (envp)
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
		if (envp)
			ft_free_tab(envp);
		reset_fd(exec);
		ft_change_exit_status(e_status, ft_itoa(131));
		g_sig = 0;
		return (131);
	}
	if (envp)
		ft_free_tab(envp);
	return (exec->status);
}
