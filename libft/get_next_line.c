/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 14:08:01 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 12:13:10 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_join_to_nl(char *s1, char *s2)
{
	char	*ret;
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	str = ft_strjoin_gnl(s1, s2);
	if (!str)
		return (NULL);
	ret = malloc(sizeof(char) * (ft_strlen_gnl(str) + 1));
	if (!ret)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\n')
		{
			ret[j++] = str[i++];
			break ;
		}
		ret[j++] = str[i++];
	}
	ret[i] = '\0';
	free(str);
	return (ret);
}

char	*ft_read(int fd, char *buffer, char *next_line, ssize_t bytes_read)
{
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		next_line = ft_strjoin_gnl(next_line, buffer);
		if (!next_line)
			return (NULL);
		if (ft_strchr_gnl(buffer, '\n'))
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (bytes_read == -1 || (bytes_read == 0 && next_line
			&& next_line[0] == '\0'))
	{
		free(next_line);
		return (NULL);
	}
	return (next_line);
}

char	*ft_set_line(char *next_line, char *line)
{
	char	*temp;

	if (next_line && next_line[0] != '\0')
	{
		temp = ft_strndup(next_line, ft_strlen_gnl(next_line));
		line = ft_join_to_nl(line, temp);
		free(temp);
		if (!line)
		{
			free(next_line);
			return (NULL);
		}
	}
	return (line);
}

static void	ft_get_line(char **line, char **next_line)
{
	char	*temp;

	*line = ft_set_line(*next_line, *line);
	if (*line)
		temp = *next_line;
	*next_line = ft_strndup(*next_line + ft_strlen_gnl(*line),
			ft_strlen_gnl(*next_line + ft_strlen_gnl(*line)));
	if (*line)
		free(temp);
}

char	*get_next_line(int fd, int error)
{
	char		*buffer;
	char		*line;
	static char	*next_line;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (error)
		return (free(next_line), NULL);
	line = NULL;
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	next_line = ft_read(fd, buffer, next_line, bytes_read);
	if (next_line && next_line[0] != '\0')
		ft_get_line(&line, &next_line);
	return (line);
}
