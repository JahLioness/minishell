/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:10:24 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/20 13:27:12 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_init_cmd_redir(t_cmd *new, char *cell, int *i)
{
	t_redir	*redir;
	char	*redir_type;

	redir = ft_init_redir();
	if (!redir)
	{
		new->redir = NULL;
		return ;
	}
	redir_type = ft_get_redir_type(cell, i);
	if (redir_type)
	{
		if (!ft_strcmp(redir_type, "<"))
			redir->type = REDIR_INPUT;
		else if (!ft_strcmp(redir_type, ">"))
			redir->type = REDIR_OUTPUT;
		else if (!ft_strcmp(redir_type, ">>"))
			redir->type = REDIR_APPEND;
		else if (!ft_strcmp(redir_type, "<<"))
			redir->type = REDIR_HEREDOC;
		free(redir_type);
	}
	else
	{
		free(redir);
		redir = NULL;
	}
	if (redir)
	{
		redir->file = ft_get_redir_file(cell, i);
		ft_redir_addback(&new->redir, redir);
	}
}

static void	ft_get_wildcard(t_token *new, int j)
{
	if (ft_check_star(new->cmd->args[j]))
		new->cmd->args = ft_wildcard_check(new->cmd->args, j);
	if (!ft_strncmp(new->cmd->args[j], "minishell:", 10))
		new->cmd->error = 1;
}

void	ft_init_token_cmd(t_token *new, char *cell, t_env **env, int *i)
{
	char	**args;
	int		j;

	if (!new)
		return ;
	new->cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!new->cmd)
		return ;
	new->cmd->error = 0;
	args = ft_get_args(cell, env, i, new->cmd);
	new->cmd->args = args;
	j = 0;
	while (new->cmd->args && new->cmd->args[j])
	{
		ft_get_wildcard(new, j);
		j++;
	}
	if (new->cmd->args && *new->cmd->args)
		new->cmd->cmd = ft_strdup(new->cmd->args[0]);
	else
		new->cmd->cmd = NULL;
	if (ft_is_redir(cell, i))
	{
		while (ft_is_redir(cell, i))
			ft_init_cmd_redir(new->cmd, cell, i);
	}
	else
		new->cmd->redir = NULL;
}
