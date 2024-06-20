/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:31:02 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/17 14:33:14 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_token_delone_utils(t_token *token)
{
	if (token->cmd)
	{
		ft_free_tab(token->cmd->args);
		free(token->cmd->cmd);
		if (token->cmd->redir)
		{
			if (token->cmd->redir->file)
				free(token->cmd->redir->file);
			free(token->cmd->redir);
		}
		free(token->cmd);
	}
}
