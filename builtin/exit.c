/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 11:33:20 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/24 19:12:59 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit_one_ok(t_ast *root, t_mini **mini, char *prompt, char **envp)
{
	char	*compare;
	long	retour;
	t_env	*status;
	t_mini	*last;

	last = ft_minilast(*mini);
	compare = ft_itoa(ft_atol(root->token->cmd->args[1]));
	ft_putendl_fd("exit", 1);
	status = ft_get_exit_status(&last->env);
	retour = ft_atol(root->token->cmd->args[1]);
	if (status)
		ft_change_exit_status(status, ft_itoa(retour % 256));
	else
		ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(retour
					% 256)));
	ft_free_exit(root, mini, envp, prompt);
	free(compare);
	return (retour % 256);
}

int	ft_exit_one_arg(t_ast *root, t_mini **mini, char *prompt, char **envp)
{
	char	*compare;
	char	*tmp;
	char	*tmp2;

	compare = ft_itoa(ft_atol(root->token->cmd->args[1]));
	if (root->token->cmd->args[1][0] == '+')
	{
		tmp = ft_strndup(&root->token->cmd->args[1][0], 1);
		tmp2 = compare;
		compare = ft_strjoin_free(tmp, tmp2);
		free(tmp2);
	}
	if (!ft_is_num(root->token->cmd->args[1]))
		return (ft_print_exit(root->token->cmd->args[1]), ft_free_exit(root,
				mini, envp, prompt), free(compare), 2);
	else if (ft_strcmp(compare, root->token->cmd->args[1]))
		return (ft_print_exit(root->token->cmd->args[1]), ft_free_exit(root,
				mini, envp, prompt), free(compare), 2);
	else
		return (free(compare), ft_exit_one_ok(root, mini, prompt, envp));
}

int	ft_exit(t_ast *root, t_mini **mini, char *prompt, char **envp)
{
	if (ft_tab_len(root->token->cmd->args) < 2)
	{
		ft_putendl_fd("exit", 1);
		ft_free_exit(root, mini, envp, prompt);
		exit(0);
	}
	else if (ft_tab_len(root->token->cmd->args) > 2)
	{
		if (!ft_is_num(root->token->cmd->args[1]))
		{
			ft_print_exit(root->token->cmd->args[1]);
			ft_free_exit(root, mini, envp, prompt);
			exit(2);
		}
		else
			return (ft_print_exit(NULL), 1);
	}
	else
		exit(ft_exit_one_arg(root, mini, prompt, envp));
}
