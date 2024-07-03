/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:41:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/03 16:16:47 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_exec_multiple_cmd(t_ast *granny, t_ast *current, t_ast *parent, t_mini **mini, char *prompt, int status)
{
	int exit_status = status;

	if (!granny || !current || !mini || !prompt)
		return (-1);
	if (current->parent)
		parent = current->parent;
	if (current->token->type == T_CMD)
	{
		printf("CMD: %s\n", current->token->cmd->cmd);
		return (ft_exec_cmd(current, granny, mini, prompt));
	}
	if (current->token->type == T_AND)
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini, prompt, exit_status);
		if (exit_status == 0)
			exit_status = ft_exec_multiple_cmd(granny, current->right, parent, mini, prompt, exit_status);
	}
	else if (current->token->type == T_OR)
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini, prompt, exit_status);
		if (exit_status != 0)
			exit_status = ft_exec_multiple_cmd(granny, current->right, parent, mini, prompt, exit_status);
	}
	else
	{
		exit_status = ft_exec_multiple_cmd(granny, current->left, parent, mini, prompt, exit_status);
		if ((parent->token->type == T_AND && exit_status == 0) || (parent->token->type == T_OR && exit_status != 0))
		{
			exit_status = ft_exec_multiple_cmd(granny, current->right, parent, mini, prompt, exit_status);
		}
	}
	return exit_status;
}

void ft_exec_token(t_mini **mini, char *prompt)
{
	t_mini *last;
	t_token *tmp;
	t_token *last_t;
	t_ast *root;

	if (!*mini)
		return;
	last = ft_minilast(*mini);
	tmp = last->tokens;
	last_t = ft_tokenlast(tmp);
	root = create_ast(tmp, last_t);
	ft_exec_multiple_cmd(root, root, root, mini, prompt, -1);
	print_ast(root, 0, ' ');
	ft_clear_ast(root);
}
