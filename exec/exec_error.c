/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:07:14 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/23 14:40:29 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ft_get_msg_error(char *msg, char *cmd, char *strerror)
{
	char	buffer[1024];

	ft_bzero(buffer, 1024);
	ft_strlcat(buffer, msg, 1024);
	ft_strlcat(buffer, cmd, 1024);
	ft_strlcat(buffer, ": ", 1024);
	ft_strlcat(buffer, strerror, 1024);
	ft_strlcat(buffer, "\n", 1024);
	return (ft_strdup(buffer));
}

void	ft_exec_cmd_error(t_exec_utils *e_utils, char **envp)
{
	if (envp)
		ft_free_tab(envp);
	if (e_utils->granny)
		ft_clear_ast(e_utils->granny);
	ft_clear_lst(e_utils->mini);
	free(e_utils->prompt);
}

void	msg_error(char *msg, char *cmd, char *strerror)
{
	char	*error;

	error = ft_get_msg_error(msg, cmd, strerror);
	write(2, error, ft_strlen(error));
	free(error);
}

int	exit_free(t_ast *granny, t_mini **mini, char **envp, char *prompt)
{
	if (envp)
		ft_free_tab(envp);
	if (granny)
		ft_clear_ast(granny);
	ft_clear_lst(mini);
	free(prompt);
	exit(EXIT_FAILURE);
}
