/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 17:56:07 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/18 17:58:19 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


// dois randomiser le nom du fichier, en parcourant
// /dev/urandom et en convertissant x char en ascii
static char	*get_name_file(int	urandom_fd)
{
	char			*name_file;
	unsigned char	random_char;
	int				i;

	name_file = malloc(sizeof(char) * 11);
	if (!name_file)
		return (NULL);
	i = 0;
	while (i < 10)
	{
		if (read(urandom_fd, &random_char, 1) != 1)
		{
			close(urandom_fd);
			free(name_file);
			return (NULL);
		}
		name_file[i] = random_char % 26 + 'a';
		i++;
	}
	name_file[i] = '\0';
	return (name_file);
}

static char	*randomize_name(void)
{
	int				urandom_fd;
	char			*name_file;

	name_file = NULL;
	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd < 0)
	{
		free(name_file);
		return (NULL);
	}
	name_file = get_name_file(urandom_fd);
	close(urandom_fd);
	return (name_file);
}

void	generate_heredoc_file(t_redir *redir)
{	
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
			redir->file_heredoc = randomize_name();
		redir = redir->next;
	}
}
