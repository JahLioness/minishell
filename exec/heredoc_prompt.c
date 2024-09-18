/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prompt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:03:22 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/18 15:39:38 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_empty_line(t_redir *current_redir, int i, int urandom_fd,
		char *line)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here_document at line ", 2);
	ft_putnbr_unsigned(i);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(current_redir->file, 2);
	ft_putendl_fd("')", 2);
	close(urandom_fd);
	free(line);
}

static void	launch_prompt_heredoc(t_cmd *cmd, t_redir *current_redir,
		t_mini *last, int urandom_fd)
{
	char	*line;
	int		i;

	i = 0;
	while (1)
	{
		ft_get_signal_heredoc();
		i++;
		line = readline("> ");
		if (ft_strchr(line, '$') && ft_strncmp(line, current_redir->file,
				ft_strlen(current_redir->file)))
			line = handle_expand_heredoc(cmd, last, line);
		if (!line)
			return (ft_empty_line(current_redir, i, urandom_fd, line));
		else if (!ft_strcmp(line, current_redir->file))
			return (close(urandom_fd), free(line));
		else
		{
			ft_putstr_fd(line, urandom_fd);
			ft_putstr_fd("\n", urandom_fd);
			free(line);
		}
	}
}

void	ft_get_heredoc(t_cmd *cmd, t_mini *last, t_redir *current_redir)
{
	int		urandom_fd;

	urandom_fd = open(current_redir->file_heredoc, O_CREAT | O_WRONLY | O_TRUNC,
			0644);
	if (urandom_fd < 0)
		return ;
	launch_prompt_heredoc(cmd, current_redir, last, urandom_fd);
}

char	*ft_get_ret_heredoc(t_env *env, char *ret, char *str, int i)
{
	if (!ft_strcmp(env->key, "_"))
	{
		ret = ft_get_value_from_varu(env, str, i, ret);
		return (ret);
	}
	ret = ft_strjoin_free(ret, env->value);
	ret = ft_strjoin_free(ret, str + i + ft_strlen(env->key));
	return (ret);
}
