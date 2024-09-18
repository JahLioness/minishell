/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 18:35:55 by andjenna          #+#    #+#             */
/*   Updated: 2024/09/18 11:41:36 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_handle_empty_first_arg(t_cmd *cmd)
{
	int	i;

	if (cmd->cmd)
		free(cmd->cmd);
	cmd->cmd = ft_strdup(cmd->args[1]);
	i = 0;
	while (cmd->args[i + 1])
	{
		free(cmd->args[i]);
		cmd->args[i] = ft_strdup(cmd->args[i + 1]);
		i++;
	}
	free(cmd->args[i]);
	cmd->args[i] = NULL;
}

void	handle_expand(t_cmd *cmd, t_mini *last)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		cmd->args[i] = ft_verif_arg(cmd->args, &last->env, cmd, i);
		i++;
	}
	cmd->args = ft_trim_quote_args(cmd->args);
	if (cmd->args && cmd->args[0])
	{
		if (cmd->cmd)
			free(cmd->cmd);
		cmd->cmd = ft_strdup(cmd->args[0]);
	}
	if (cmd->args && cmd->args[0] && !*cmd->args[0] && cmd->args[1])
		ft_handle_empty_first_arg(cmd);
}

int	ft_cmdsize(t_cmd *cmd)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
