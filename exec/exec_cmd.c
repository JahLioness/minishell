/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/18 19:39:58 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// cat >file = comme un heredoc on ecrit dans le file jusqu'a ce qu'on fasse ctrl
//	+ D ou ctrl + C ou ctrl + "\""
//  ls && ls - l (echo hey || echo bye) <-- syntaxe error non geree
// je dois paufiner heredoc --> randomiser le nom de fichier,
//	le faire executer en premier si il y a d'autre commande avant
// les pipes et la norminette a faire.
// je vais changer les gestion du heredoc, a part des files.
// si MINI->heredoc = 1,
// creation du here_doc
// si plusieurs heredoc,
//	(token->redir->next). recreer un nouveau fichier a chaque fois (mais la creation du fichier se fait dans le parent et l'ouverture dans le fils)
// a anticiper cat devant plusieurs heredoc
// probleme expand --> echo $USER "$USER" $USER
// HEREDOC EST OK MAIS PLUS LES FD, JE SAIS PAS CE QUE J'AI FOUTU

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
void	unlink_files(t_redir *redir)
{
	t_redir	*tmp;
	tmp = redir;

	while (tmp)
	{
		if (tmp->type == REDIR_HEREDOC && access(tmp->file_heredoc, F_OK) == 0)
			unlink(tmp->file_heredoc);
		tmp = tmp->next;
	}
}
int	ft_exec_cmd_path(t_ast *root, t_ast *granny, t_mini **mini, char **envp,
		char *prompt)
{
	char	*cmd_path;
	t_mini	*last;

	last = ft_minilast(*mini);
	if (!root->token->cmd->cmd || !*root->token->cmd->cmd)
	{
		ft_free_tab(envp);
		ft_clear_lst(mini);
		if (root)
			ft_clear_ast(root);
		free(prompt);
		return (0);
	}
	if (!ft_strcmp(root->token->cmd->cmd, "$_"))
		cmd_path = ft_tabchr(envp, "_", '=');
	else if (ft_strchr(root->token->cmd->cmd, '/'))
	{
		cmd_path = ft_strdup(root->token->cmd->cmd);
		if (access(cmd_path, F_OK | X_OK) == -1 && errno == EACCES)
		{
			ft_putendl_fd("Minishell : Permission denied", 2);
			free(cmd_path);
			if (envp)
				ft_free_tab(envp);
			if (root)
				ft_clear_ast(root);
			ft_clear_lst(mini);
			free(prompt);
			exit(126);
		}
	}
	else
		cmd_path = ft_get_cmd_path_env(root->token->cmd->cmd, envp);
	if (root->token->cmd->redir)
		unlink_files(root->token->cmd->redir);
	if (!cmd_path)
	{
		if (execve(root->token->cmd->cmd, root->token->cmd->args, envp) == -1)
			ft_exec_cmd_error(granny, mini, envp, prompt);
	}
	else
	{
		if (execve(cmd_path, root->token->cmd->args, envp) == -1)
		{
			free(cmd_path);
			ft_exec_cmd_error(granny, mini, envp, prompt);
		}
	}
	return (0);
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
				&last->env, root->token->cmd, i);
		i++;
	}
	root->token->cmd->args = ft_trim_quote_args(root->token->cmd->args);
	if (root->token->cmd->cmd)
		free(root->token->cmd->cmd);
	if (root->token->cmd->args[0])
		root->token->cmd->cmd = ft_strdup(root->token->cmd->args[0]);
	if (!*root->token->cmd->args[0] && root->token->cmd->args[1])
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
		if (root->token->cmd->redir && root->token->cmd->redir->type != REDIR_HEREDOC && access(root->token->cmd->redir->file, R_OK | W_OK | X_OK) == -1)
		{
			exec->error_ex = 1;
			exec->redir_fd = -1;
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(root->token->cmd->redir->file, 2);
			ft_putendl_fd(": permission denied", 2);
		}
		// premier appel de la fonction pour verifier les file
		else if (root->token->cmd->redir
			&& root->token->cmd->redir->type != REDIR_HEREDOC)
		{
			ft_handle_redir_file(cmd);
			if (exec->redir_fd != -1)
			{
				close(exec->redir_fd);
				exec->redir_fd = -1;
			}
		}
		// cas de redirection pour "cat file" sans sympbole de redirection
		else if (!root->token->cmd->redir
			&& !ft_strcmp(root->token->cmd->args[0], "cat")
			&& !root->token->cmd->redir && exec->redir_fd == -1
			&& root->token->cmd->args[1])
		{
			if (access(root->token->cmd->args[1], R_OK) == -1)
			{
				printf("1\n");
				exec->error_ex = 1;
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(root->token->cmd->args[1], 2);
				ft_putendl_fd(": permission denied", 2);
			}
			else
			{
				printf("2\n");
				exec->redir_fd = open(root->token->cmd->args[1], O_RDONLY);
				if (exec->redir_fd < 0)
				{
					ft_putstr_fd("minishell: ", 2);
					ft_putstr_fd(root->token->cmd->args[1], 2);
					ft_putendl_fd(": No such file or directory", 2);
					exec->error_ex = 1;
				}
				close(exec->redir_fd);
				exec->redir_fd = -1;
			}
		}
		// si erreur de file, on execute pas le reste des commandes
		if (exec->error_ex == 1 || (root->token->cmd->redir
				&& root->token->cmd->redir->type != REDIR_HEREDOC
				&& !root->token->cmd->cmd))
		{
			if (exec->redir_fd != -1)
			{
				close(exec->redir_fd);
				exec->redir_fd = -1;
			}
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
				{
					if (tmp->type == REDIR_APPEND)
					{
						exec->redir_fd = open(tmp->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
						if (exec->redir_fd < 0)
						{
							ft_putstr_fd("minishell: ", 2),
							ft_putstr_fd(tmp->file, 2),
							ft_putendl_fd(": No such file or directory", 2);
						}
					}
					else if (tmp->type == REDIR_INPUT)
					{
						exec->redir_fd = open(tmp->file, O_RDONLY, 0644);
						if (exec->redir_fd < 0)
						{
							ft_putstr_fd("minishell: ", 2),
							ft_putstr_fd(tmp->file, 2),
							ft_putendl_fd(": No such file or directory", 2);
						}
						else
						{
							close(exec->redir_fd);
							exec->redir_fd = STDOUT_FILENO;
						}
					}
					else if (tmp->type == REDIR_OUTPUT)
					{
						exec->redir_fd = open(tmp->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
						if (exec->redir_fd < 0)
						{
							ft_putstr_fd("minishell: ", 2),
							ft_putstr_fd(tmp->file, 2),
							ft_putendl_fd(": No such file or directory", 2);
						}
					}
				}
				else
					exec->redir_fd = STDOUT_FILENO;
				exec->status = ft_exec_builtin(root->token, &last->env,
						exec->redir_fd);
				if (exec->redir_fd != -1 && exec->redir_fd != STDOUT_FILENO)
				{
					close (exec->redir_fd);
					exec->redir_fd = -1;
				}
				ft_free_tab(envp);
				envp = NULL;
				while (tmp && tmp->file)
				{
					if (exec->redir_fd != STDOUT_FILENO && exec->redir_fd != -1)
					{
						close(exec->redir_fd);
						exec->redir_fd = -1;
					}
					tmp = tmp->next;
				}
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
				exec->redir_fd = -1;
				exec->pid = fork();
				if (exec->pid < 0)
					return (ft_putendl_fd("minishell: fork failed", 2), 1);
				if (exec->pid == 0)
				{
					// deuxieme appel de la fonction pour verifier les file et here_doc
					if (root->token->cmd->redir)
						ft_handle_redir_file(cmd);
					if (root->token->cmd->redir && (exec->redir_fd != -1
							&& exec->redir_fd != STDOUT_FILENO))
					{
						if (root->token->cmd->redir->type == REDIR_INPUT)
							dup2(exec->redir_fd, STDIN_FILENO);
						else if (root->token->cmd->redir->type == REDIR_OUTPUT)
							dup2(exec->redir_fd, STDOUT_FILENO);
						else if (root->token->cmd->redir->type == REDIR_APPEND)
							dup2(exec->redir_fd, STDOUT_FILENO);
						else if (root->token->cmd->redir->type == REDIR_HEREDOC)
							dup2(exec->redir_fd, STDIN_FILENO);
						if (close(exec->redir_fd) == -1)
							perror("close in dup2");
						exec->redir_fd = -1;
					}
					// cas de redirection pour "cat file" sans sympbole de redirection
					else if (!root->token->cmd->redir
						&& !ft_strcmp(root->token->cmd->args[0], "cat")
						&& exec->redir_fd == -1 && root->token->cmd->args[1])
					{
						exec->redir_fd = open(root->token->cmd->args[1],
								O_RDONLY);
						if (exec->redir_fd < 0)
						{
							ft_putstr_fd("minishell: ", 2);
							ft_putstr_fd(root->token->cmd->args[1], 2);
							ft_putendl_fd(": No shuch file or directory", 2);
							ft_free_tab(envp);
							ft_clear_lst(mini);
							if (granny)
								ft_clear_ast(granny);
							free(prompt);
							exit(EXIT_FAILURE);
						}
						dup2(exec->redir_fd, STDIN_FILENO);
						close(exec->redir_fd);
						exec->redir_fd = -1;
					}
					if (root->token->cmd->redir && exec->redir_fd != -1
						&& exec->redir_fd != STDOUT_FILENO)
					{
						if (close(exec->redir_fd) == -1)
							perror("close");
					}
					exec->redir_fd = -1;
					// execution de la commande
					ft_exec_cmd_path(root, granny, mini, envp, prompt);
					exit(EXIT_SUCCESS);
				}
				else
				{
					if (exec->redir_fd != -1 && exec->redir_fd != STDOUT_FILENO)
						close(exec->redir_fd);
					exec->redir_fd = -1;
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
						ft_change_exit_status(e_status,
							ft_itoa(WEXITSTATUS(exec->status)));
						ft_free_tab(envp);
						return (WEXITSTATUS(exec->status));
					}
				}
			}
		}
	}
	
	e_status = ft_get_exit_status(&last->env);
	if (g_sig == SIGQUIT)
	{
		if (close(exec->redir_fd) == -1)
			perror("close crtl-\\");
		exec->redir_fd = -1;
		ft_change_exit_status(e_status, ft_itoa(131));
		ft_free_tab(envp);
		g_sig = 0;
		return (131);
	}
	if (envp)
		ft_free_tab(envp);
	return (exec->status);
}
