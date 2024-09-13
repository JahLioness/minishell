/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 13:07:14 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/27 16:04:11 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char	*underscore;
// underscore = ft_tabchr(envp, "_", '=');
// ft_putstr_fd("minishell: ", 2);
// if (!ft_strcmp(last->tokens->cmd->cmd, "$_"))
// 	ft_putstr_fd(underscore, 2);
// else
// 	ft_putstr_fd(last->tokens->cmd->cmd, 2);
// ft_putstr_fd(": command not found\n", 2);
// free(underscore);

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
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(" : ", 2);
	ft_putendl_fd(strerror, 2);
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
