/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 12:54:14 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/18 17:12:11 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_redir_file(char *str, int *i)
{
	int		j;
	char	*ret;

	while (str[*i] && ft_is_whitespaces(str[*i]))
		(*i)++;
	j = *i;
	while (str[*i] && ft_is_not_stop(str[*i]) && !ft_is_whitespaces(str[*i]))
		(*i)++;
	ret = ft_substr(str, j, *i - j);
	return (ret);
}

char	*ft_get_redir_type(char *str, int *i)
{
	char	*ret;

	while (str[*i])
	{
		if (((str[*i] == '>' && str[*i + 1] == '>') || (str[*i] == '<' && str[*i
						+ 1] == '<')) && str[*i + 1])
		{
			ret = ft_substr(str, *i, 2);
			*i += 2;
			return (ret);
		}
		else if ((str[*i + 1] && ((str[*i] == '>' && str[*i + 1] != '>')
					|| (str[*i] == '<' && str[*i + 1] != '<'))) || (!str[*i + 1]
				&& (str[*i] == '<' || str[*i] == '>')))
		{
			ret = ft_substr(str, *i, 1);
			*i += 1;
			return (ret);
		}
		(*i)++;
	}
	return (NULL);
}

int	ft_is_redir(char *str, int *i)
{
	int	j;

	j = *i;
	while (str[j] && str[j] != '&' && str[j] != '|')
	{
		if (((str[j] == '>' && str[j + 1] == '>') || (str[j] == '<' && str[j
						+ 1] == '<')) && str[j + 1])
			return (1);
		else if ((str[j + 1] && ((str[j] == '>' && str[j + 1] != '>')
					|| (str[j] == '<' && str[j + 1] != '<'))) || (!str[j + 1]
				&& (str[j] == '<' || str[j] == '>')))
			return (1);
		j++;
	}
	return (0);
}

t_token	*ft_redir_token(t_token *token)
{
	t_token	*redir;
	t_token	*tmp;

	tmp = token;
	redir = NULL;
	while (tmp)
	{
		if (tmp->cmd && tmp->cmd->redir)
		{
			redir = tmp;
			break ;
		}
		tmp = tmp->next;
	}
	return (redir);
}

char	**ft_check_redir_args(t_token *token)
{
	t_token	*redir;
	char	**ret;
	int		len;

	redir = ft_redir_token(token);
	if (redir)
	{
		len = ft_get_redir_args_len(redir);
		if (len == 0)
			return (NULL);
		ret = ft_calloc(ft_tab_len(redir->cmd->args) + len + 1, sizeof(char *));
		if (!ret)
			return (NULL);
		ret = ft_check_redir_args_utils(redir, ret);
		return (ret);
	}
	return (NULL);
}
