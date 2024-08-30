/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_wildcard_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 18:55:12 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/26 14:04:07 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_wildcard_len(char **args, int j)
{
	DIR				*dir;
	struct dirent	*entry;
	int				len;

	dir = opendir(".");
	if (!dir)
		return (perror("opendir"), 0);
	entry = readdir(dir);
	len = 0;
	while (entry)
	{
		if (ft_compare(args[j], entry->d_name))
			len++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (len);
}

char	**ft_init_wildcard(char **args, int j)
{
	char	**wildcard;
	int		i;
	int		len;

	len = ft_wildcard_len(args, j);
	i = 0;
	while (args[i])
		i++;
	wildcard = ft_calloc(i + len + 1, sizeof(char *));
	if (!wildcard)
		return (NULL);
	return (wildcard);
}

int	ft_check_star(char *arg)
{
	int		i;
	char	quote;

	if (!arg)
		return (0);
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '"' || arg[i] == '\'')
		{
			quote = arg[i];
			i++;
			while (arg[i] && arg[i] != quote)
				i++;
			if (arg[i])
				i++;
		}
		if (arg[i] && arg[i] == '*')
			return (1);
		if (arg[i] && arg[i] != '"' && arg[i] != '\'')
			i++;
	}
	return (0);
}

void	ft_skip_m_s_idx(int *i, int j, int *s_idx, int *m_idx)
{
	*s_idx = (*i)++;
	*m_idx = j;
}
