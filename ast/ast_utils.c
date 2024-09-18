/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 12:20:45 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 11:40:35 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_bracket(t_token *token)
{
	t_token	*tmp_prev;
	int		i;

	if (!token || !token->prev)
		return (0);
	tmp_prev = token->prev;
	i = 0;
	while (tmp_prev)
	{
		if (tmp_prev->type == O_BRACKET)
			i++;
		else if (tmp_prev->type == C_BRACKET)
			i--;
		tmp_prev = tmp_prev->prev;
	}
	return (i);
}

void	ft_clear_ast(t_ast *root)
{
	if (root == NULL)
		return ;
	root->parent = NULL;
	ft_clear_ast(root->left);
	ft_clear_ast(root->right);
	free(root);
}

t_ast	*ast_init(void)
{
	t_ast	*ast;

	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->token = NULL;
	ast->parent = NULL;
	ast->left = NULL;
	ast->right = NULL;
	return (ast);
}

t_ast	*ast_new_node(t_token *token)
{
	t_ast	*ast;

	if (!token)
		return (NULL);
	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	ast->token = token;
	ast->parent = NULL;
	ast->left = NULL;
	ast->right = NULL;
	return (ast);
}

t_ast	*create_operator_node(t_token *token, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->token = token;
	node->parent = NULL;
	node->left = left;
	node->left->parent = node;
	node->right = right;
	node->right->parent = node;
	return (node);
}
