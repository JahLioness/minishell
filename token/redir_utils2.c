/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:35:49 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 11:58:21 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_get_redir_args_len(t_token *redir)
{
	t_token	*tmp;
	int		len;
	int		i;

	len = 0;
	tmp = redir->next;
	while (tmp && tmp->cmd && !tmp->cmd->redir)
	{
		i = 0;
		while (tmp->cmd->args[i])
		{
			len++;
			i++;
		}
		tmp = tmp->next;
	}
	return (len);
}

char	**ft_check_redir_args_utils(t_token *redir, char **ret)
{
	t_token	*tmp;
	t_token	*next;
	int		len;
	int		i;

	len = 0;
	if (!redir->cmd->args)
		return (NULL);
	while (redir->cmd->args[len])
	{
		ret[len] = ft_strdup(redir->cmd->args[len]);
		len++;
	}
	next = redir->next;
	while (next && next->cmd && !next->cmd->redir)
	{
		i = 0;
		tmp = next;
		while (next->cmd->args[i])
			ret[len++] = ft_strdup(next->cmd->args[i++]);
		next = next->next;
		ft_token_delone(tmp);
	}
	ret[len] = NULL;
	return (ret);
}

t_redir	*ft_init_redir(void)
{
	t_redir	*redir;

	redir = ft_calloc(sizeof(t_redir), 1);
	if (!redir)
		return (NULL);
	redir->type = 4;
	redir->fd_redir = -1;
	redir->file = NULL;
	redir->file_heredoc = NULL;
	return (redir);
}

t_redir	*ft_redirlast(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return (NULL);
	tmp = redir;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_redir_addback(t_redir **redir, t_redir *new)
{
	t_redir	*last;

	if (!redir || !new)
		return ;
	if (!*redir)
		*redir = new;
	else
	{
		last = ft_redirlast(*redir);
		last->next = new;
		new->prev = last;
	}
}
