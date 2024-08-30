/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 11:51:33 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/22 13:28:45 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*ft_check_logic_op(t_token *current, t_token *root)
{
	if (current->type == T_AND || current->type == T_OR)
	{
		if (!root || ft_check_bracket(root) > ft_check_bracket(current))
			root = current;
	}
	return (root);
}

static t_token	*ft_find_root(t_token *first, t_token *last)
{
	t_token	*current;
	t_token	*root;

	current = last;
	root = NULL;
	while (current && current != first)
	{
		root = ft_check_logic_op(current, root);
		current = current->prev;
	}
	return (root);
}

static t_ast	*ft_create_cmd_node(t_token *token)
{
	t_ast	*new_node;

	if (!token)
		return (NULL);
	while (token->type == O_BRACKET)
		token = token->next;
	new_node = ast_new_node(token);
	if (!new_node)
		return (NULL);
	return (new_node);
}

static t_ast	*ft_create_subtree(t_token *first, t_token *last, t_token *root)
{
	t_token	*left_hand;
	t_token	*right_hand;
	t_ast	*left;
	t_ast	*right;

	left_hand = root->prev;
	right_hand = root->next;
	left = create_ast(first, left_hand);
	right = create_ast(right_hand, last);
	return (create_operator_node(root, left, right));
}

t_ast	*create_ast(t_token *first, t_token *last)
{
	t_token	*root;

	root = ft_find_root(first, last);
	if (!root)
		return (ft_create_cmd_node(first));
	else
		return (ft_create_subtree(first, last, root));
}
