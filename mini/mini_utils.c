/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:46:46 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/24 19:19:51 by andjenna         ###   ########.fr       */
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

static int	ft_is_double_op(t_token *token)
{
	if (!token || (token->type == T_CMD && !token->cmd))
		return (0);
	return ((token->type == T_AND || token->type == T_AND_E
			|| token->type == T_OR || (token->type == T_CMD && token->cmd->redir
				&& !*token->cmd->redir->file) || token->type == T_PIPE)
		&& token->next && (token->next->type == T_AND
			|| token->next->type == T_AND_E || token->next->type == T_OR
			|| (token->next->type == T_CMD && token->next->cmd->redir
				&& !*token->next->cmd->redir->file)
			|| token->next->type == T_PIPE));
}

static int	ft_verif_condition_token(t_token *tmp)
{
	if ((ft_is_double_op(tmp) || ((!tmp->prev || (tmp->prev
						&& tmp->prev->type == O_BRACKET)) && (tmp->type == T_AND
					|| tmp->type == T_OR || tmp->type == T_PIPE
					|| tmp->type == T_AND_E))))
		return (1);
	return (0);
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
					&& tmp->next->type != T_CMD
					&& tmp->next->type != O_BRACKET) || !tmp->next))
			bracket--;
		else if (ft_verif_condition_token(tmp))
			return (ft_check_operator(tmp));
		if (!ft_verif_line(tmp))
			return (0);
		tmp = tmp->next;
	}
	if (bracket != 0)
		return (ft_putstr_fd("minishell: syntax error near ", 2),
			ft_putendl_fd("unexpected token `newline'", 2), 0);
	return (1);
}

t_mini	*ft_new_mini_part(t_mini *new)
{
	t_token	*redir;
	t_env	*e_status;
	char	**args;

	ft_init_token(new, new->cell);
	if (!ft_verif_tokens(new))
	{
		e_status = ft_get_exit_status(&new->env);
		ft_change_exit_status(e_status, ft_itoa(2));
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
	ft_is_heredoc(new);
	return (new);
}
