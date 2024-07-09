/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 16:45:28 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/09 14:42:48 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

char *randomize_name(void)
{
	int urandom_fd;
	char *name_file;
	unsigned char random_char;
	int i;

	i = 0;
	name_file = malloc(sizeof(char) * 11);
	if (!name_file)
		return (NULL);
	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd < 0)
	{
		free(name_file);
		return NULL;
	}
	while (i < 10)
	{
		if (read(urandom_fd, &random_char, 1) != 1)
		{
			close(urandom_fd);
			free(name_file);
			return NULL;
		}
		name_file[i] = random_char % 26 + 'a';
		i++;
	}
	name_file[i] = '\0';
	close(urandom_fd);
	return (name_file);
}

char *handle_expand_heredoc(t_ast *root, t_mini *last, char *line)
{
	char **args_heredoc;
	int i;
	int	j;

	i = 0;
	j = 0;
	if (!line)
		return (NULL);
	args_heredoc = ft_get_args(line, &j);
	while (args_heredoc[i])
	{
		printf("args_heredoc[%d]: %s\n", i, args_heredoc[i]);
		i++;
	}
	if (!args_heredoc)
		return (NULL);
	i = 0;
	while (args_heredoc[i])
	{
		args_heredoc[i] = ft_verif_arg(args_heredoc, &last->env, root->token->cmd, i);
		printf("VERIF_ARG -> args_heredoc[%d]: %s\n", i, args_heredoc[i]);
		i++;
	}
	args_heredoc = ft_trim_quote_args(args_heredoc);
	if (line)
		free(line);
	i = 0;
	while (args_heredoc[i])
	{
		if (i == 0)
			line = ft_strdup(args_heredoc[i]);
		else
		{
			
			line = ft_strjoin(line, " ");
			line = ft_strjoin(line, args_heredoc[i]);
		}
		i++;
	}
	// line = ft_strdup(args_heredoc[0]);
	ft_free_tab(args_heredoc);
	return (line);
}

char *ft_get_heredoc(t_redir * redir, t_ast * root, t_mini * last)
{
	int urandom_fd;
	char *line;
	char *name_file;
	int i;

	line = NULL;
	i = 0;
	// dois randomiser le nom du fichier, en parcourant /dev/urandom et en convertissant x char en ascii
	name_file = randomize_name();
	urandom_fd = open(name_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (urandom_fd < 0)
		return (free(name_file), NULL);
	// name_file[i] = NULL;
	while (1)
	{
		ft_get_signal_heredoc();
		i++;
		line = readline("> ");
		if (g_sig == 2)
			break ;
		if (ft_strchr(line, '$'))
			line = handle_expand_heredoc(root, last, line);
		if (!line)
		{
			ft_putstr_fd("minishell: warning: ", 2);
			ft_putstr_fd("here_document at line ", 2);
			ft_putnbr_unsigned(i);
			ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
			ft_putstr_fd(redir->file, 2);
			ft_putendl_fd("')", 2);
			break;
		}
		else if (!ft_strcmp(line, redir->file))
			break;
		else
		{
			ft_putstr_fd(line, urandom_fd);
			ft_putstr_fd("\n", urandom_fd);
			free(line);
		}
	}
	close(urandom_fd);
	free(line);
	return (name_file);
}