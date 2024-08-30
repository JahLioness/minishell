/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_init_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:10:24 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/30 17:29:36 by ede-cola         ###   ########.fr       */
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

void	ft_check_redir_arg(t_cmd *new, char *cell, int *i)
{
	int		j;
	int		k;
	char	**new_args;

	j = 0;
	if (!new || !new->args)
		return ;
	new_args = ft_calloc(ft_tab_len(new->args) + 2, sizeof(char *));
	while (new->args && new->args[j])
	{
		new_args[j] = ft_strdup(new->args[j]);
		j++;
	}
	ft_free_tab(new->args);
	new->args = new_args;
	while (cell[*i] && ft_is_not_stop(cell[*i]))
	{
		if (cell[*i])
		{
			k = ft_get_index_arg_utils(cell, i);
			new->args[j] = ft_strndup(cell + k, (*i - k));
			j++;
		}
		if (cell[*i] && ft_is_not_stop(cell[*i]) && cell[*i] != '$'
			&& cell[*i] != 123 && cell[*i] != '"' && cell[*i] != '\''
			&& !ft_isalnum(cell[*i]))
			(*i)++;
	}
	
}

void	ft_init_cmd_redir(t_cmd *new, char *cell, int *i)
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
		ft_check_redir_arg(new, cell, i);
		ft_redir_addback(&new->redir, redir);
	}
	if (redir)
		redir->file_heredoc = NULL;
}

void	ft_get_wildcard(t_token *new, int j)
{
	if (ft_check_star(new->cmd->args[j]))
		new->cmd->args = ft_wildcard_check(new->cmd->args, j);
	if (!ft_strncmp(new->cmd->args[j], "minishell:", 10))
		new->cmd->error = 1;
}

void	init_exec(t_cmd *cmd)
{
	cmd->exec.error_ex = 0;
	cmd->exec.pid = -1;
	cmd->exec.prev_fd = -1;
	cmd->exec.redir_in = -1;
	cmd->exec.redir_out = -1;
	cmd->exec.status = 0;
}

void	ft_init_token_cmd(t_token *new, char *cell, int *i)
{
	char	**args;
	int		j;

	if (!new)
		return ;
	printf("cell = %s\n", cell + *i);
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
		while (ft_is_redir(cell, i))
			ft_init_cmd_redir(new->cmd, cell, i);
	else
		new->cmd->redir = NULL;
	init_exec(new->cmd);
	ft_init_token_cmd_pipe(new, new->cmd, cell, i);
}
