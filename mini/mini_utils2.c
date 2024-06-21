/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:48:22 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/21 13:47:50 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

	tmp = mini->tokens;
	while (tmp)
	{
		if (tmp->type == T_CMD && tmp->cmd && tmp->cmd->redir
			&& tmp->cmd->redir->type == REDIR_HEREDOC)
		{
			mini->is_heredoc = 1;
			return ;
		}
		tmp = tmp->next;
	}
}
