/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:10:24 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/25 16:21:59 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_set_redir_type(t_redir *redir, char *redir_type)
{
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
}

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
	ft_set_redir_type(redir, redir_type);
	if (redir)
	{
		redir->file = ft_get_redir_file(cell, i, NULL);
		ft_redir_addback(&new->redir, redir);
	}
	if (redir)
		redir->file_heredoc = NULL;
}

static void	ft_get_wildcard(t_token *new, int j)
{
	if (ft_check_star(new->cmd->args[j]))
		new->cmd->args = ft_wildcard_check(new->cmd->args, j);
	if (!ft_strncmp(new->cmd->args[j], "minishell:", 10))
		new->cmd->error = 1;
}

static t_exec	*init_exec(void)
{
	t_exec	*exec;

	exec = ft_calloc(sizeof(t_exec), 1);
	if (!exec)
		return (NULL);
	exec->error_ex = 0;
	exec->pid = -1;
	exec->prev_fd = -1;
	exec->redir_fd = -1;
	exec->redir_in = -1;
	exec->redir_out = -1;
	exec->status = 0;
	return (exec);
}

void	ft_init_token_cmd(t_token *new, char *cell, int *i)
{
	char	**args;
	int		j;

	if (!new)
		return ;
	new->cmd = ft_calloc(sizeof(t_cmd), 1);
	if (!new->cmd)
		return ;
	new->cmd->cmd = NULL;
	new->cmd->error = 0;
	args = ft_get_args(cell, i);
	new->cmd->args = args;
	j = 0;
	while (new->cmd->args && new->cmd->args[j])
		ft_get_wildcard(new, j++);
	if (new->cmd->args && *new->cmd->args)
		new->cmd->cmd = ft_strdup(new->cmd->args[0]);
	if (ft_is_redir(cell, i))
	{
		while (ft_is_redir(cell, i))
			ft_init_cmd_redir(new->cmd, cell, i);
	}
	else
		new->cmd->redir = NULL;
	new->cmd->exec = init_exec();
}
