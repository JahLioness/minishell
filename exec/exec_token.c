/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:41:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/21 19:17:59 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_exec_multiple_cmd(t_ast *root, t_ast *current, t_ast *parent, t_mini **mini, char *prompt, int status)
{
	int exit_status = status;

	if (!root || !current || !mini || !prompt)
		return (-1);
	if (current->parent)
		parent = current->parent;
	if (current->token->type == T_CMD)
	{
		printf("CMD: %s\n", current->token->cmd->cmd);
		return (ft_exec_cmd(current, mini, prompt));
	}
	if (current->token->type == T_AND)
	{
		exit_status = ft_exec_multiple_cmd(root, current->left, parent, mini, prompt, exit_status);
		if (exit_status == 0)
			exit_status = ft_exec_multiple_cmd(root, current->right, parent, mini, prompt, exit_status);
	}
	else if (current->token->type == T_OR)
	{
		exit_status = ft_exec_multiple_cmd(root, current->left, current, mini, prompt, exit_status);
		if (exit_status != 0)
			exit_status = ft_exec_multiple_cmd(root, current->right, parent, mini, prompt, exit_status);
	}
	else
	{
		exit_status = ft_exec_multiple_cmd(root, current->left, current, mini, prompt, exit_status);
		if ((parent->token->type == T_AND && exit_status == 0) || (parent->token->type == T_OR && exit_status != 0))
		{
			exit_status = ft_exec_multiple_cmd(root, current->right, parent, mini, prompt, exit_status);
		}
	}
	return exit_status;
}

// int ft_exec_multiple_cmd(t_ast *root, t_ast *granny, t_mini **mini, char *prompt, int status)
// {
// 	int exit_status;

// 	exit_status = status;
// 	if (!root || !granny || !mini || !prompt)
// 		return (-1);
// 	if (root->left->token->type == T_CMD && root->right->token->type == T_CMD)
// 	{
// 		exit_status = ft_exec_cmd(root->left, mini, prompt);
// 		if (exit_status == 0 && root->left->parent->token->type == T_AND)
// 			exit_status = ft_exec_cmd(root->right, mini, prompt);
// 		else if (exit_status != 0 && root->left->parent->token->type == T_OR)
// 			exit_status = ft_exec_cmd(root->right, mini, prompt);
// 		else
// 			return (exit_status);
// 	}
// 	else if (root->left->token->type != T_CMD && root->right->token->type == T_CMD)
// 	{
// 		exit_status = ft_exec_multiple_cmd(root->left, root, mini, prompt, exit_status);
// 		if (exit_status == 0 && root->left->parent->token->type == T_AND)
// 			exit_status = ft_exec_cmd(root->right, mini, prompt);
// 		else if (exit_status != 0 && root->left->parent->token->type == T_OR)
// 			exit_status = ft_exec_cmd(root->right, mini, prompt);
// 		else
// 			return (exit_status);
// 	}
// 	else if (root->left->token->type == T_CMD && root->right->token->type != T_CMD)
// 	{
// 		exit_status = ft_exec_cmd(root->left, mini, prompt);
// 		if (exit_status == 0 && root->left->parent->token->type == T_AND)
// 			exit_status = ft_exec_multiple_cmd(root->right, granny, mini, prompt, exit_status);
// 		else if (exit_status != 0 && root->left->parent->token->type == T_OR)
// 			exit_status = ft_exec_multiple_cmd(root->right, granny, mini, prompt, exit_status);
// 		else
// 			return (exit_status);
// 	}
// 	else if (root->left->token->type != T_CMD && root->right->token->type != T_CMD)
// 	{	
// 		exit_status = ft_exec_multiple_cmd(root->left, root, mini, prompt, exit_status);
// 	}
// 	return (exit_status);
// }

void choose_exec(t_ast *root, t_mini **mini, char *prompt)
{
	if (!root || !mini || !prompt)
		return;
	if (root->token->type == T_CMD)
	{
		ft_exec_cmd(root, mini, prompt);
		return;
	}
	else
		ft_exec_multiple_cmd(root, root, root, mini, prompt, -1);
}

void ft_exec_token(t_mini **mini, char *prompt)
{
	t_mini *last;
	t_token *tmp;
	t_token *last_t;
	t_ast *root;
	t_ast *granny;

	if (!*mini)
		return;
	last = ft_minilast(*mini);
	tmp = last->tokens;
	last_t = ft_tokenlast(tmp);
	root = create_ast(tmp, last_t);
	granny = root;
	choose_exec(root, mini, prompt);
	// unfold_ast(root, -1, &last->env, mini, prompt);
	// print_ast(root, 0, ' ');
	ft_clear_ast(root);
}
