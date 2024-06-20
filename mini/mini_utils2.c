/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 10:48:22 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/20 12:48:57 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_verif_line(t_token *token)
{
	if ((token->type == T_AND || token->type == T_OR || token->type == T_PIPE
			|| token->type == O_BRACKET) && !token->next)
		return (ft_check_operator(token));
	else if (token->type == O_BRACKET)
	{
		while (token->type == O_BRACKET)
			token = token->next;
		if (!token)
			return (ft_putendl_fd("minishell: syntax error near unexpected token `('",
					2), 0);
		else if (token->type == C_BRACKET)
		{
			while (token->type == C_BRACKET)
				token = token->next;
			if (!token)
				return (ft_putendl_fd("minishell: syntax error near unexpected token `)'",
						2), 0);
		}
	}
	else if (token->type == T_CMD && token->cmd && token->cmd->redir
		&& (!token->cmd->redir->file || !*token->cmd->redir->file))
		return (ft_putstr_fd("minishell: syntax error near ", 2),
			ft_putendl_fd("unexpected token `newline'", 2), 0);
	return (1);
}
