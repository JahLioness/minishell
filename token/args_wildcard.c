/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:01:54 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/06 12:29:41 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_skip_star(char *patter, char *str, int *i, int *j)
{
	while (patter[*i] == '*')
		(*i)++;
	while (str[*j] && str[*j] != patter[*i])
		(*j)++;
}

static void	ft_skip_equal_index(char *pattern, int *i, int *j)
{
	if (pattern[*i + 1] != '\0')
		(*i)++;
	(*j)++;
}

int	ft_compare(char *pattern, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (pattern[i] == '*' && !pattern[i + 1])
		return (1);
	while (str[j])
	{
		if (pattern[i] == '*')
			ft_skip_star(pattern, str, &i, &j);
		else if (pattern[i] == str[j])
			ft_skip_equal_index(pattern, &i, &j);
		else
		{
			if (i > 0 && pattern[i - 1] == '*')
				j++;
			else
				return (0);
		}
		if (str[j] == pattern[i] && !str[j + 1] && (!pattern[i] || !pattern[i
					+ 1]))
			return (1);
	}
	return (0);
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
		return (args);
	while (args[j + 1])
	{
		wildcard[w.i] = ft_strdup(args[j + 1]);
		w.i++;
		j++;
	}
	ft_free_tab(args);
	return (wildcard);
}
