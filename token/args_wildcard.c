/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:01:54 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/17 10:38:14 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_skip_idx(int *i, int *j)
{
	(*i)++;
	(*j)++;
}

static int	ft_skip_star(char *pattern, int *i)
{
	while (pattern[*i] == '*')
		(*i)++;
	return (*i);
}

int	ft_compare(char *pattern, char *str)
{
	int	i;
	int	j;
	int	star_idx;
	int	match_idx;

	i = 0;
	j = 0;
	star_idx = -1;
	match_idx = 0;
	while (str[j])
	{
		if (pattern[i] == '*')
			ft_skip_m_s_idx(&i, j, &star_idx, &match_idx);
		else if (pattern[i] == str[j])
			ft_skip_idx(&i, &j);
		else if (star_idx != -1)
		{
			i = star_idx + 1;
			j = ++match_idx;
		}
		else
			return (0);
	}
	i = ft_skip_star(pattern, &i);
	return (pattern[i] == '\0');
}

char	**ft_compare_entry(char **args, t_wildcard *w, int j, char **wildcard)
{
	while (w->entry)
	{
		if (ft_compare(args[j], w->entry->d_name) && w->entry->d_name[0] != '.')
		{
			if (w->i == 0)
			{
				while (args[w->i] && args[w->i] != args[j])
				{
					wildcard[w->i] = ft_strdup(args[w->i]);
					w->i++;
				}
				wildcard[w->i] = ft_strdup(w->entry->d_name);
			}
			else
				wildcard[w->i] = ft_strdup(w->entry->d_name);
			w->i++;
		}
		w->entry = readdir(w->dir);
	}
	return (wildcard);
}

char	**ft_wildcard_check(char **args, int j)
{
	char		**wildcard;
	t_wildcard	w;

	w.dir = opendir(".");
	if (!w.dir)
		return (perror("opendir"), NULL);
	wildcard = ft_init_wildcard(args, j);
	if (!wildcard)
		return (NULL);
	w.entry = readdir(w.dir);
	w.i = 0;
	wildcard = ft_compare_entry(args, &w, j, wildcard);
	closedir(w.dir);
	if (w.i == 0)
		return (ft_free_tab(wildcard), args);
	while (args[j + 1])
	{
		wildcard[w.i] = ft_strdup(args[j + 1]);
		w.i++;
		j++;
	}
	wildcard[w.i] = NULL;
	ft_free_tab(args);
	return (wildcard);
}
