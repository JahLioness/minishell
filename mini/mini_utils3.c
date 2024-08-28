/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 13:55:33 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/28 15:38:36 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_set_heredoc_node(t_mini *mini)
{
	mini->is_heredoc = 1;
	return ;
}

int	ft_is_pipe_alone(t_token *token)
{
	t_cmd	*tmp;

	tmp = token->cmd;
	while (tmp)
	{
		if (tmp->pipe && (!tmp->next || (!tmp->next->cmd && !tmp->next->redir)))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static void	ft_redir_loop(t_redir *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_INPUT && (!redir->file || !*redir->file))
			ft_putendl_fd("<'", 2);
		else if (redir->type == REDIR_OUTPUT && (!redir->file || !*redir->file))
			ft_putendl_fd(">'", 2);
		else if (redir->type == REDIR_APPEND && (!redir->file || !*redir->file))
			ft_putendl_fd(">>'", 2);
		else if (redir->type == REDIR_HEREDOC && (!redir->file
				|| !*redir->file))
			ft_putendl_fd("<<'", 2);
		redir = redir->next;
	}
}

void	ft_cmd_syntax_error(t_token *token)
{
	t_cmd	*tmp;
	t_redir	*redir;

	tmp = token->cmd;
	while (tmp)
	{
		if (tmp->redir)
		{
			redir = tmp->redir;
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_redir_loop(redir);
		}
		else if (!tmp->cmd || !*tmp->cmd || ft_is_pipe_alone(token))
			ft_putendl_fd("minishell: syntax error near unexpected token `|'",
				2);
		tmp = tmp->next;
	}
}
