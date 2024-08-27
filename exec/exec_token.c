/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:41:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/27 16:01:16 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_heredoc(t_ast *current)
{
	int	exit_status;

	if (current->token->type == T_CMD && !current->token->cmd->cmd
		&& current->token->cmd->redir
		&& current->token->cmd->redir->type == REDIR_HEREDOC)
	{
		exit_status = 0;
		return (exit_status);
	}
	return (-1);
}

int	ft_exec_multiple_cmd(t_exec_utils *e_utils, t_ast *current)
{
	int	exit_status;

	if (!e_utils->granny || !current || !e_utils->mini || !e_utils->prompt)
		return (-1);
	if (current->parent)
		e_utils->parent = current->parent;
	exit_status = ft_check_heredoc(current);
	if (current->token->type == T_CMD)
		return (ft_exec_cmd(current, e_utils));
	if (current->token->type == T_AND)
	{
		exit_status = ft_exec_multiple_cmd(e_utils, current->left);
		if (exit_status == 0)
			return (ft_exec_multiple_cmd(e_utils, current->right));
	}
	else if (current->token->type == T_OR)
	{
		exit_status = ft_exec_multiple_cmd(e_utils, current->left);
		if (exit_status != 0)
			return (ft_exec_multiple_cmd(e_utils, current->right));
	}
	return (exit_status);
}

t_cmd	*ft_return_heredoc(t_cmd *cmd, t_redir *redir)
{
	t_cmd	*heredoc;

	heredoc = NULL;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			heredoc = cmd;
			break ;
		}
		redir = redir->next;
	}
	return (heredoc);
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
			node_heredoc = ft_return_heredoc(tmp_cmd, tmp_redir);
		}
		tmp_token = tmp_token->next;
	}
	return (node_heredoc);
}

void	ft_exec_token(t_mini **mini, char *prompt)
{
	t_mini			*last;
	t_token			*tmp;
	t_token			*last_t;
	t_exec_utils	exec_utils;
	t_cmd			*node_heredoc;

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
	exec_utils.granny = create_ast(last->tokens, last_t);
	exec_utils.parent = NULL;
	exec_utils.mini = mini;
	exec_utils.prompt = prompt;
	ft_exec_multiple_cmd(&exec_utils, exec_utils.granny);
	print_ast(exec_utils.granny, 0, ' ');
	ft_clear_ast(exec_utils.granny);
}
