/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:41:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/26 02:04:47 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//IL FAUT FAIRE CETTE FOMCTION AUTREMENT
int	ft_exec_multiple_cmd(t_ast *granny, t_ast *current, t_ast *parent,
		t_mini **mini, char *prompt, int status)
{
	int	exit_status;

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
	if (current->token->type == T_CMD && parent->token->type != T_PIPE)
		return (ft_exec_cmd(current, granny, mini, prompt));
	else if (current->token->type == T_CMD && parent->token->type == T_PIPE)
	{
		exit_status = ft_exec_pipe(parent->left, granny, mini, prompt);
		exit_status = ft_exec_pipe(parent->right, granny, mini, prompt);
	}
	if (current->token->type == T_AND)
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini,
				prompt, exit_status);
		if (exit_status == 0)
			exit_status = ft_exec_multiple_cmd(granny, current->right, parent,
					mini, prompt, exit_status);
	}
	else if (current->token->type == T_OR)
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini,
				prompt, exit_status);
		if (exit_status != 0)
			exit_status = ft_exec_multiple_cmd(granny, current->right, parent,
					mini, prompt, exit_status);
	}
	else if (current->token->type == T_PIPE)
		exit_status = ft_exec_multiple_cmd(granny,current->left, parent, mini, prompt, exit_status);
	else
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini,
				prompt, exit_status);
		if ((parent->token->type == T_AND && exit_status == 0)
			|| (parent->token->type == T_OR && exit_status != 0))
		{
			exit_status = ft_exec_multiple_cmd(granny, current->right, parent,
					mini, prompt, exit_status);
		}
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
