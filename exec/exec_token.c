/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:41:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 15:39:12 by ede-cola         ###   ########.fr       */
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

t_cmd	*get_heredoc_node(t_cmd *cmd)
{
	t_redir	*tmp_redir;
	t_cmd	*node_heredoc;

	node_heredoc = NULL;
	if (cmd && cmd->redir)
	{
		tmp_redir = cmd->redir;
		while (tmp_redir)
		{
			if (tmp_redir->type == REDIR_HEREDOC)
			{
				node_heredoc = cmd;
				break ;
			}
			tmp_redir = tmp_redir->next;
		}
	}
	return (node_heredoc);
}

void	ft_exec_token(t_mini **mini, char *prompt)
{
	t_mini			*last;
	t_token			*last_t;
	t_exec_utils	exec_utils;

	if (!*mini)
		return ;
	last = ft_minilast(*mini);
	last_t = ft_tokenlast(last->tokens);
	if (last->is_heredoc)
		ft_get_heredoc_loop(last->tokens, mini, prompt);
	exec_utils.granny = create_ast(last->tokens, last_t);
	exec_utils.parent = NULL;
	exec_utils.mini = mini;
	exec_utils.prompt = prompt;
	ft_exec_multiple_cmd(&exec_utils, exec_utils.granny);
	ft_clear_ast(exec_utils.granny);
}
