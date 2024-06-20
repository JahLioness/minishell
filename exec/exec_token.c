/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 10:41:30 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/19 18:17:37 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exec_token(t_mini **mini, char *prompt)
{
	t_mini	*last;
	t_token	*tmp;
	t_token	*last_t;
	t_ast	*root;

	(void)prompt;
	if (!*mini)
		return ;
	last = ft_minilast(*mini);
	tmp = last->tokens;
	last_t = ft_tokenlast(tmp);
	root = create_ast(tmp, last_t);
	unfold_ast(root, -1, &last->env, mini, prompt);
	print_ast(root, 0, ' ');
	ft_clear_ast(root);
}
