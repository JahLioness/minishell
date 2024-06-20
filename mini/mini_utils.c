/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:46:46 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/20 16:06:49 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_quote(char *line, int i)
{
	char	quote;

	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
			{
				ft_putstr_fd("minishell: syntax error near ", 2);
				ft_putendl_fd("unexpected token `newline'", 2);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

int	ft_check_operator(t_token *token)
{
	if (token->type == T_AND)
		ft_putendl_fd("minishell: syntax error near unexpected token `&&'", 2);
	else if (token->type == T_AND_E)
		ft_putendl_fd("minishell: syntax error near unexpected token `&'", 2);
	else if (token->type == T_OR)
		ft_putendl_fd("minishell: syntax error near unexpected token `||'", 2);
	else if (token->type == T_CMD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (token->cmd->redir->type == REDIR_INPUT)
			ft_putendl_fd("<'", 2);
		else if (token->cmd->redir->type == REDIR_OUTPUT)
			ft_putendl_fd(">'", 2);
		else if (token->cmd->redir->type == REDIR_APPEND)
			ft_putendl_fd(">>'", 2);
		else if (token->cmd->redir->type == REDIR_HEREDOC)
			ft_putendl_fd("<<'", 2);
	}
	else if (token->type == T_PIPE)
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
	return (0);
}

static int	ft_is_double_op(t_token *token)
{
	if (!token || (token->type == T_CMD && !token->cmd))
		return (0);
	return ((token->type == T_AND || token->type == T_AND_E || token->type == T_OR
			|| (token->type == T_CMD && token->cmd->redir
				&& !*token->cmd->redir->file) || token->type == T_PIPE)
		&& token->next && (token->next->type == T_AND || token->next->type == T_AND_E
			|| token->next->type == T_OR || (token->next->type == T_CMD
				&& token->next->cmd->redir && !*token->next->cmd->redir->file)
			|| token->next->type == T_PIPE));
}

int	ft_verif_tokens(t_mini *mini)
{
	t_token	*tmp;
	int		bracket;

	tmp = mini->tokens;
	bracket = 0;
	while (tmp)
	{
		if (tmp->type == O_BRACKET)
			bracket++;
		else if (tmp->type == C_BRACKET && ((tmp->next
					&& tmp->next->type != T_CMD && tmp->next->type != O_BRACKET)
				|| !tmp->next))
			bracket--;
		else if (ft_is_double_op(tmp))
			return (ft_check_operator(tmp));
		if (!ft_verif_line(tmp))
			return (0);
		tmp = tmp->next;
	}
	if (bracket != 0)
	{
		return (ft_putstr_fd("minishell: syntax error near ", 2),
			ft_putendl_fd("unexpected token `newline'", 2), 0);
	}
	return (1);
}

t_mini	*ft_new_mini_part(t_mini **mini, t_mini *new)
{
	t_mini	*last;
	t_token	*redir;
	char	**args;

	last = ft_minilast(*mini);
	if (last)
	{
		new->env = ft_env_copy(last->env);
		new->prev = last;
	}
	ft_init_token(new, new->cell);
	if (!ft_verif_tokens(new))
	{
		new->error = 1;
		return (new);
	}
	redir = ft_redir_token(new->tokens);
	args = ft_check_redir_args(new->tokens);
	if (args && *args)
	{
		ft_free_tab(redir->cmd->args);
		redir->cmd->args = args;
	}
	return (new);
}
