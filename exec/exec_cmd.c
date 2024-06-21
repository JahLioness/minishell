/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/21 13:27:56 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// cat >file = comme un heredoc on ecrit dans le file jusqu'a ce qu'on fasse ctrl + D ou ctrl + C ou ctrl + "\""
//  ls && ls - l (echo hey || echo bye) <-- syntaxe error non geree
// je dois paufiner heredoc --> randomiser le nom de fichier, le faire executer en premier si il y a d'autre commande avant 
// les pipes et la norminette a faire.

int ft_exec_builtin(t_token *token, t_env **env, int fd)
{
	t_env *status;
	int exit_status;

	exit_status = -1;
	if (!ft_strcmp(token->cmd->cmd, "echo"))
		exit_status = ft_echo(fd, ft_get_args_echo(token->cmd->args, env), ft_get_flag_echo(token->cmd->args));
	else if (!ft_strcmp(token->cmd->cmd, "cd"))
		exit_status = ft_cd(token->cmd->args[1], env);
	else if (!ft_strcmp(token->cmd->cmd, "pwd"))
		exit_status = ft_pwd(fd, env);
	else if (!ft_strcmp(token->cmd->cmd, "export"))
		exit_status = ft_exec_export(token, env, fd);
	else if (!ft_strcmp(token->cmd->cmd, "unset"))
		exit_status = ft_exec_unset(token->cmd, env);
	else if (!ft_strcmp(token->cmd->cmd, "env"))
		exit_status = ft_print_env(env, fd);
	status = ft_get_exit_status(env);
	if (status)
		ft_change_exit_status(status, ft_itoa(exit_status));
	else
		ft_envadd_back(env, ft_envnew(ft_strdup("?"), ft_itoa(exit_status)));
	return (exit_status);
}

int ft_exec_cmd_path(t_ast *root, t_mini **mini, char **envp, char *prompt)
{
	char *cmd_path;
	t_mini *last;

	last = ft_minilast(*mini);
	if (!root->token->cmd->cmd)
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
		cmd_path = ft_strdup(root->token->cmd->cmd);
	else
		cmd_path = ft_get_cmd_path_env(root->token->cmd->cmd, envp);
	if (!cmd_path)
	{
		if (execve(root->token->cmd->cmd, root->token->cmd->args, envp) == -1)
			ft_exec_cmd_error(root, mini, envp, prompt);
	}
	else
	{
		if (execve(cmd_path, root->token->cmd->args, envp) == -1)
		{
			free(cmd_path);
			ft_exec_cmd_error(root, mini, envp, prompt);
		}
	}
	return (0);
}

int ft_is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

int ft_get_heredoc(t_redir *redir)
{
	int urandom_fd;
	int fd_stdin;
	char *line;

	line = NULL;
	// dois randomiser le nom du fichier, en parcourant /dev/urandom et en convertissant x char en ascii (NOT DONE YET)
	urandom_fd = open(".txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (urandom_fd < 0)
		return (5);
	fd_stdin = dup(STDIN_FILENO);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, redir->file))
			break;
		else
		{
			ft_putstr_fd(line, urandom_fd);
			ft_putstr_fd("\n", urandom_fd);
			free(line);
		}
	}
	close(urandom_fd);
	close(fd_stdin);
	free(line);
	return (0);
}

int ft_handle_redir_file(int redir_fd, t_ast *root)
{
	t_redir *current;

	current = root->token->cmd->redir;
	while (current)
	{
		if (current->type == REDIR_INPUT)
		{
			if (redir_fd != -1)
				close(redir_fd);
			redir_fd = open(current->file, O_RDONLY);
			if (redir_fd < 0)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(current->file, 2), ft_putendl_fd(": No shuch file or directory", 2), 5);
		}
		else if (current->type == REDIR_OUTPUT)
		{
			if (redir_fd != -1)
				close(redir_fd);
			redir_fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (redir_fd < 0)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(current->file, 2), ft_putendl_fd(": No shuch file or directory", 2), 5);
		}
		else if (current->type == REDIR_APPEND)
		{
			if (redir_fd != -1)
				close(redir_fd);
			redir_fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (redir_fd < 0)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(current->file, 2), ft_putendl_fd(": No shuch file or directory", 2), 5);
		}
		else if (current->type == REDIR_HEREDOC)
		{
			//creer le fichier here doc
			ft_get_heredoc(current);
			redir_fd = open(".txt", O_RDONLY);
			if (redir_fd < 0)
				return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(current->file, 2), ft_putendl_fd(": No shuch file or directory", 2), 5);
		}
		current = current->next;
	}
	return (redir_fd);
}

int ft_exec_cmd(t_ast *root, t_mini **mini, char *prompt)
{
	t_mini *last;
	pid_t pid;
	int status;
	int fd;
	int redir_fd;
	t_env *e_status;
	char **envp;
	int error;

	fd = -1;
	redir_fd = -1;
	status = -1;
	error = 0;
	last = ft_minilast(*mini);
	envp = ft_get_envp(&last->env);
	//gestion des quotes et expand
	if (root->token->type == T_CMD && root->token->cmd)
	{
		int i = 0;
		if ((root->token->cmd->cmd && root->token->cmd->args) || (!root->token->cmd->cmd && *root->token->cmd->args))
		{
			while (root->token->cmd->args[i])
			{
				root->token->cmd->args[i] = ft_verif_arg(root->token->cmd->args, &last->env, root->token->cmd, i);
				i++;
			}
			root->token->cmd->args = ft_trim_quote_args(root->token->cmd->args);
			if (root->token->cmd->cmd)
				free(root->token->cmd->cmd);
			root->token->cmd->cmd = ft_strdup(root->token->cmd->args[0]);
		}
		//premier appel de la fonction pour verifier les file
		if (root->token->cmd->redir && root->token->cmd->redir->type != REDIR_HEREDOC)
			redir_fd = ft_handle_redir_file(redir_fd, root);
		//si erreur de file, on execute pas le reste des commandes
		if (redir_fd == 5)
		{
			e_status = ft_get_exit_status(&last->env);
			if (e_status)
				ft_change_exit_status(e_status, ft_itoa(1));
			else
				ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(1)));
			error = 1;
		}
		ft_set_var_underscore(root->token->cmd->args, &last->env, envp);
		//si pas de file de redirection, on redirige vers la sortie standard
		if (redir_fd == -1)
			redir_fd = STDOUT_FILENO;
		if (!error)
		{
			if (ft_is_builtin(root->token->cmd->cmd))
			{
				status = ft_exec_builtin(root->token, &last->env, redir_fd);
				if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
					close(redir_fd);
				ft_free_tab(envp);
				envp = NULL;
			}
			else if (!ft_strcmp(root->token->cmd->cmd, "exit"))
			{
				status = ft_exit(root, mini, prompt, envp);
				ft_free_tab(envp);
				envp = NULL;
			}
			else
			{
				redir_fd = -1;
				pid = fork();
				if (pid < 0)
					return (ft_putendl_fd("minishell: fork failed", 2), 1);
				if (pid == 0)
				{
					//deuxieme appel de la fonction pour verifier les file et here_doc
					redir_fd = ft_handle_redir_file(redir_fd, root);
					if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
					{
						if (root->token->cmd->redir->type == REDIR_INPUT)
							dup2(redir_fd, STDIN_FILENO);
						else if (root->token->cmd->redir->type == REDIR_OUTPUT)
							dup2(redir_fd, STDOUT_FILENO);
						else if (root->token->cmd->redir->type == REDIR_APPEND)
							dup2(redir_fd, STDOUT_FILENO);
						else if (root->token->cmd->redir->type == REDIR_HEREDOC)
						{
							dup2(redir_fd, STDIN_FILENO);
						}
						if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
							close(redir_fd);
						unlink(".txt");
					}
					//cas de redirection pour "cat file" sans sympbole de redirection
					else if (redir_fd == -1 && root->token->cmd->args[1])
					{
						fd = open(root->token->cmd->args[1], O_RDONLY);
						if (fd < 0)
						{
							ft_putstr_fd("minishell: ", 2);
							ft_putstr_fd(root->token->cmd->args[1], 2);
							ft_putendl_fd(": No shuch file or directory", 2);
							ft_free_tab(envp);
							ft_clear_lst(mini);
							if (root)
								ft_clear_ast(root);
							free(prompt);
							exit(EXIT_FAILURE);
						}
						dup2(fd, STDIN_FILENO);
						close(fd);
					}
					//execution de la commande
					ft_exec_cmd_path(root, mini, envp, prompt);
					exit(EXIT_SUCCESS);
				}
				else
				{
					if (redir_fd != -1 && redir_fd != STDOUT_FILENO)
						close(redir_fd);
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))
					{
						e_status = ft_get_exit_status(&last->env);
						if (e_status)
							ft_change_exit_status(e_status, ft_itoa(WEXITSTATUS(status)));
						else
							ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(WEXITSTATUS(status))));
						ft_free_tab(envp);
						return (WEXITSTATUS(status));
					}
				}
			}
		}
	}
	if (envp)
		ft_free_tab(envp);
	return (status);
}
