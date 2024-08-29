/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:48:22 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/29 12:17:11 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_operator(t_token *token)
{
	if (token->type == T_AND)
		ft_putendl_fd("minishell: syntax error near unexpected token `&&'", 2);
	else if (token->type == T_AND_E)
		ft_putendl_fd("minishell: syntax error near unexpected token `&'", 2);
	else if (token->type == T_OR)
		ft_putendl_fd("minishell: syntax error near unexpected token `||'", 2);
	else if (token->type == T_CMD)
		ft_cmd_syntax_error(token);
	return (0);
}

int	ft_verif_brack_f_line(t_token *token)
{
	while (token && token->type == O_BRACKET)
		token = token->next;
	if (!token)
		return (ft_putstr_fd("minishell: syntax error near ", 2),
			ft_putendl_fd("unexpected token `('", 2), 0);
	else if (token->type == C_BRACKET)
	{
		while (token && token->type == C_BRACKET)
			token = token->next;
		if (!token)
			return (ft_putstr_fd("minishell: syntax error near ", 2),
				ft_putendl_fd("unexpected token `)'", 2), 0);
	}
	return (1);
}

int	ft_verif_line(t_token *token)
{
	if ((token->type == T_AND || token->type == T_OR || token->type == T_PIPE
			|| token->type == O_BRACKET || token->type == T_AND_E)
		&& !token->next)
		return (ft_check_operator(token));
	else if (token->type == T_CMD && token->next
		&& token->next->type == O_BRACKET)
		return (ft_putstr_fd("minishell: syntax error near ", 2),
			ft_putendl_fd("unexpected token `('", 2), 0);
	else if (token->type == O_BRACKET)
		return (ft_verif_brack_f_line(token));
	else if (token->type == T_CMD && token->cmd && token->cmd->redir
		&& (!token->cmd->redir->file || !*token->cmd->redir->file))
		return (ft_putstr_fd("minishell: syntax error near ", 2),
			ft_putendl_fd("unexpected token `newline'", 2), 0);
	return (1);
}

void	ft_is_heredoc(t_mini *mini)
{
	t_token	*tmp;
	t_cmd	*tmp_cmd;

	tmp = mini->tokens;
	while (tmp)
	{
		if (tmp->type == T_CMD)
		{
			tmp_cmd = tmp->cmd;
			while (tmp_cmd)
			{
				ft_set_heredoc_node(tmp_cmd, mini);
				tmp_cmd = tmp_cmd->next;
			}
		}
		tmp = tmp->next;
	}
}

int	ft_check_redir_file(t_token *token)
{
	t_token	*tmp;
	t_cmd	*tmp_cmd;
	t_redir	*tmp_redir;

	tmp = token;
	while (tmp)
	{
		tmp_cmd = tmp->cmd;
		while (tmp_cmd)
		{
			tmp_redir = tmp_cmd->redir;
			while (tmp_redir)
			{
				if (!tmp_redir->file || !*tmp_redir->file)
					return (1);
				tmp_redir = tmp_redir->next;
			}
			tmp_cmd = tmp_cmd->next;
		}
		tmp = tmp->next;
	}
	return (0);
}
