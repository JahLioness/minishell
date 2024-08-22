/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:18:04 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/22 12:59:15 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	ft_print_token_lst(t_token *token)
{
	t_token			*tmp;
	t_cmd			*tmp_cmd;
	t_redir			*tmp_redir;
	int				i;
	size_t			j;
	
	tmp = token;
	i = 0;
	while (tmp)
	{
		printf("TOKEN: %d\n", i);
		printf("type: ");
		switch (tmp->type)
		{
			case T_CMD:
				printf("CMD\n");
				break;
			case T_AND:
				printf("&&\n");
				break;
			case T_OR:
				printf("||\n");
				break;
			case T_PIPE:
				printf("|\n");
				break;
			case O_BRACKET:
				printf("(\n");
				break;
			case C_BRACKET:
				printf(")\n");
				break;
			default:
				break;
		}
		if (tmp->cmd)
		{
			tmp_cmd = tmp->cmd;
			while (tmp_cmd)
			{
				if (tmp_cmd->cmd && *(tmp_cmd->cmd))
					printf("CMD: %s\n", tmp_cmd->cmd);
				if (tmp_cmd->args)
				{
					j = 0;
					while (j < ft_tab_len(tmp_cmd->args))
					{
						printf("ARG[%zu]: %s\n", j, tmp_cmd->args[j]);
						j++;
					}
				}
				if (tmp_cmd->redir)
				{
					tmp_redir = tmp_cmd->redir;
					while (tmp_redir)
					{
						switch (tmp_redir->type)
						{
							case REDIR_INPUT:
								printf("REDIR: <\n");
								break;
							case REDIR_OUTPUT:
								printf("REDIR: >\n");
								break;
							case REDIR_APPEND:
								printf("REDIR: >>\n");
								break;
							case REDIR_HEREDOC:
								printf("REDIR: <<\n");
								break;
							default:
								break;
						}
						printf("FILE: %s\n", tmp_redir->file);
						tmp_redir = tmp_redir->next;
					}
				}
				printf("ERROR: %d\n", tmp_cmd->error);
				tmp_cmd = tmp_cmd->next;
			}
		}
		tmp = (tmp)->next;
		i++;
	}
}

void	ft_print_lst(t_mini *mini)
{
	t_mini	*tmp_mini;
	int				i;
	
	tmp_mini = mini;
	i = 0;
	while (tmp_mini)
	{
		printf("MINI: %d\n", i);
		printf("cell: %s\n", tmp_mini->cell);
		printf("MINI ERROR: %d\n", tmp_mini->error);
		printf("MINI is heredoc = %d\n", tmp_mini->is_heredoc);
		ft_print_token_lst(tmp_mini->tokens);
		// ft_print_env(&tmp_mini->env, 1);
		tmp_mini = tmp_mini->next;
		i++;
	}
}

void print_ast(t_ast *node, int depth, char c)
{
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; ++i) {
        printf("    ");
    }
	printf("%c: ", c);

    if (node->token->type == T_CMD) 
	{
		if (node->parent)
		{
			printf("Parent: ");
			switch (node->parent->token->type) 
			{
				case T_AND:
					printf("&&; ");
					break;
				case T_OR:
					printf("||; ");
					break;
				case T_PIPE:
					printf("|; ");
					break;
				default:
					break;
			}
		}
        printf("CMD: %s", node->token->cmd->cmd);
		printf(" %s", node->token->cmd->args[1]);
		if (node->token->cmd->redir)
		{
			switch (node->token->cmd->redir->type)
			{
				case REDIR_INPUT:
					printf(" < ");
					break;
				case REDIR_OUTPUT:
					printf(" > ");
					break;
				case REDIR_APPEND:
					printf(" >> ");
					break;
				case REDIR_HEREDOC:
					printf(" << ");
					break;
				default:
					break;
			}
			printf("%s", node->token->cmd->redir->file);
		}
		printf("\n");
    } 
	else 
	{
		printf("Parent: ");
		if (node->parent)
		{
			switch (node->parent->token->type) 
			{
				case T_AND:
					printf("&&; ");
					break;
				case T_OR:
					printf("||; ");
					break;
				case T_PIPE:
					printf("|; ");
					break;
				default:
					break;
			}
		}
		else
			printf("(null);");
        switch (node->token->type) 
		{
            case T_AND:
                printf("&&\n");
                break;
            case T_OR:
                printf("||\n");
                break;
            case T_PIPE:
                printf("|\n");
                break;
            default:
                break;
        }
        print_ast(node->left, depth + 1, 'L');
        print_ast(node->right, depth + 1, 'R');
    }
}
