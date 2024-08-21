/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:01:09 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/21 15:14:52 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_fd(t_exec *exec)
{
	if (exec->redir_out != -1 && exec->redir_out != STDOUT_FILENO)
	{
		close(exec->redir_out);
		exec->redir_out = -1;
	}
	if (exec->redir_in != -1 && exec->redir_in != STDIN_FILENO)
	{
		close(exec->redir_in);
		exec->redir_in = -1;
	}
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

void	handle_redir(t_ast *root, t_mini **mini, t_env *e_status)
{
	t_cmd	*cmd;
	t_exec	*exec;
	t_mini	*last;

	cmd = root->token->cmd;
	exec = cmd->exec;
	last = ft_minilast(*mini);
	if (root->token->cmd->redir
		&& root->token->cmd->redir->type != REDIR_HEREDOC)
	{
		ft_handle_redir_file(cmd);
		reset_fd(exec);
	}
	// cas de redirection pour "cat file" sans sympbole de redirection
	else if (!root->token->cmd->redir && root->token->cmd->args
			&& !ft_strcmp(root->token->cmd->args[0], "cat")
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
}

void	handle_redir_dup(t_ast *root, t_exec *exec, t_cmd *cmd)
{
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
	else if (!root->token->cmd->redir && !ft_strcmp(root->token->cmd->args[0],
				"cat") && root->token->cmd->args[1])
	{
		cat_wt_symbole(root->token->cmd, exec);
		dup2(exec->redir_in, STDIN_FILENO);
		dup2(exec->redir_out, STDOUT_FILENO);
	}
}
