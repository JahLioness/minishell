/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:59:34 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/12 13:39:05 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_pipe_init(char *line, int *i)
{
	*i = ft_check_whitespace(line, *i);
	if (line[*i] && line[*i] == '|' && line[*i + 1] != '|')
	{
		(*i)++;
		if (line[*i] && ft_is_whitespaces(line[*i]))
			*i = ft_check_whitespace(line, *i);
		return (1);
	}
	return (0);
}

void	ft_set_cmd_redir(char *cell, int *i, t_cmd *cmd)
{
	if (ft_is_redir(cell, i))
		while (ft_is_redir(cell, i))
			ft_init_cmd_redir(cmd, cell, i);
	else
		cmd->redir = NULL;
}

void	ft_init_token_cmd_pipe(t_token *new, t_cmd *cmd, char *cell, int *i)
{
	char	**args;
	int		j;

	(void)new;
	while (ft_is_pipe_init(cell, i))
	{
		cmd->pipe = 1;
		cmd->next = ft_calloc(sizeof(t_cmd), 1);
		if (!cmd->next)
			return ;
		cmd->next->cmd = NULL;
		cmd->next->error = 0;
		args = ft_get_args(cell, i);
		cmd->next->args = args;
		j = 0;
		while (cmd->next->args && cmd->next->args[j])
			ft_get_wildcard(cmd->next, j++);
		if (cmd->next->args && *cmd->next->args)
			cmd->next->cmd = ft_strdup(cmd->next->args[0]);
		ft_set_cmd_redir(cell, i, cmd->next);
		init_exec(cmd->next);
		cmd = cmd->next;
	}
}
