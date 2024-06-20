/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_root.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:04:38 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/17 15:51:29 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unfold_ast(t_ast *root, int status, t_env **env, t_mini **mini, char *prompt)
{
	t_ast *tmp;
	t_ast	*left;
	t_ast	*right;
	int		exit_status;

	if (!root)
		return ;
	tmp = root;
	left = NULL;
	right = NULL;
	exit_status = status;
	if (!root)
		return ;
	if (root->token->type == T_AND || root->token->type == T_OR || root->token->type == T_PIPE)
	{
		left = root->left;
		right = root->right;
	}
	else if (root->token->type == T_CMD)
		exit_status = ft_exec_cmd(root, mini, prompt);
	if (left && left->token->type == T_CMD)
		exit_status = ft_exec_cmd(left, mini, prompt);
	if (exit_status == 0 && root->token->type == T_AND)
		exit_status = ft_exec_cmd(right, mini, prompt);
	else if (left && left->token->type == T_CMD && exit_status == -1 && root->token->type == T_OR)
		exit_status = ft_exec_cmd(right, mini, prompt);
	else if (left && left->token->type == T_CMD && exit_status == 0 && root->token->type == T_PIPE)
		exit_status = ft_exec_cmd_pipe(right, mini, prompt);
	else if (left && (left->token->type == T_AND || left->token->type == T_OR || left->token->type == T_PIPE))
		unfold_ast(left, exit_status, env, mini, prompt);
	return ;
}
