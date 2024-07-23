/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:06:46 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/23 12:48:21 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_clear_lst(t_mini **mini)
{
	t_mini	*tmp;

	while (*mini)
	{
		tmp = *mini;
		*mini = (*mini)->next;
		if (tmp->cell)
			free(tmp->cell);
		ft_clear_token(&tmp->tokens);
		ft_clearenv(&tmp->env);
		free(tmp);
	}
}

t_mini	*ft_minilast(t_mini *mini)
{
	t_mini	*tmp;

	if (!mini)
		return (NULL);
	tmp = mini;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_miniadd_back(t_mini **mini, t_mini *new)
{
	t_mini	*last;

	if (!mini || !new)
		return ;
	if (!*mini)
		*mini = new;
	else
	{
		last = ft_minilast(*mini);
		last->next = new;
	}
}

t_mini	*ft_mini_init(void)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->cell = NULL;
	mini->env = NULL;
	mini->error = 0;
	mini->is_heredoc = 0;
	mini->tokens = NULL;
	mini->prev = NULL;
	mini->next = NULL;
	return (mini);
}

t_mini	*ft_new_mini(char *line, t_mini **mini)
{
	t_mini	*new;
	t_mini	*last;
	t_env	*e_status;

	last = ft_minilast(*mini);
	new = ft_mini_init();
	if (!new)
		return (NULL);
	if (last)
	{
		new->env = ft_env_copy(last->env);
		new->prev = last;
	}
	if (!ft_check_quote(line, 0) || !line[ft_check_whitespace(line, 0)])
	{
		e_status = ft_get_exit_status(&new->env);
		ft_change_exit_status(e_status, ft_itoa(2));
		new->error = 1;
		return (new);
	}
	new->cell = ft_trim_empty_quotes(line);
	new = ft_new_mini_part(new);
	return (new);
}
