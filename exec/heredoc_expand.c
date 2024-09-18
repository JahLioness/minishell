/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:57:37 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/18 15:37:57 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_quotes(char *str, char *ret, int i)
{
	while (str[i])
	{
		if (str[i] == '"')
		{
			ret = ft_strjoin_free(ret, "\"");
			break ;
		}
		else if (str[i] == '\'')
		{
			ret = ft_strjoin_free(ret, "'");
			break ;
		}
		i++;
	}
	return (ret);
}

static int	expand_heredoc_utils(char *str, int *i)
{
	int		k;
	char	quote;

	k = *i;
	quote = 0;
	while (str[*i] && !ft_is_whitespaces(str[*i]) && ft_is_not_stop(str[*i])
		&& !quote)
	{
		if (str[*i] == '"' || str[*i] == '\'')
		{
			quote = str[*i];
			(*i)++;
			while (str[*i] && str[*i] != quote)
				(*i)++;
			if (str[*i] == quote)
				(*i)++;
			quote = 0;
		}
		else
			ft_check_acco(str, i);
		if (str[*i])
			(*i)++;
	}
	return (k);
}

static char	**heredoc_args(char *cell, int *i)
{
	char	**args;
	int		j;
	int		k;

	args = ft_calloc(ft_countwords_args(cell + *i) + 1, sizeof(char *));
	if (!args)
		return (NULL);
	j = 0;
	while (cell[*i] && ft_is_not_stop(cell[*i]))
	{
		if (cell[*i])
		{
			k = expand_heredoc_utils(cell, i);
			args[j] = ft_strndup(cell + k, (*i - k));
			if (!args[j])
				return (ft_free_split(j, args), NULL);
			j++;
		}
		if (cell[*i] && ft_is_not_stop(cell[*i]) && cell[*i] != '$'
			&& cell[*i] != 123 && cell[*i] != '"' && cell[*i] != '\''
			&& !ft_isalnum(cell[*i]))
			(*i)++;
	}
	args[j] = NULL;
	return (args);
}

static char	*ft_expand_heredoc_utils(char **args_heredoc)
{
	char	*tmp_line;
	char	*new_line;
	char	*line;
	int		i;

	i = 0;
	line = NULL;
	tmp_line = NULL;
	new_line = NULL;
	while (args_heredoc[i])
	{
		if (i == 0)
			line = ft_strdup(args_heredoc[i]);
		else
		{
			tmp_line = ft_strjoin(line, " ");
			free(line);
			new_line = ft_strjoin(tmp_line, args_heredoc[i]);
			free(tmp_line);
			line = new_line;
		}
		i++;
	}
	return (line);
}

char	*handle_expand_heredoc(t_cmd *cmd, t_mini *last, char *line)
{
	char	**args_heredoc;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!line)
		return (NULL);
	args_heredoc = heredoc_args(line, &j);
	if (!args_heredoc)
		return (NULL);
	while (args_heredoc[i])
	{
		args_heredoc[i] = ft_verif_arg_heredoc(args_heredoc, &last->env, cmd,
				i);
		i++;
	}
	if (line)
		free(line);
	line = ft_expand_heredoc_utils(args_heredoc);
	return (ft_free_tab(args_heredoc), line);
}
