/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 11:33:20 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 13:59:17 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit_one_ok(t_exec_utils *e_utils, t_cmd *cmd)
{
	char	*compare;
	long	retour;
	t_env	*status;
	t_mini	*last;

	last = ft_minilast(*e_utils->mini);
	compare = ft_itoa(ft_atoll(cmd->args[1]));
	if (!e_utils->current->token->cmd->next)
		ft_putendl_fd("exit", 1);
	status = ft_get_exit_status(&last->env);
	retour = ft_atoll(cmd->args[1]);
	if (status)
		ft_change_exit_status(status, ft_itoa(retour % 256));
	else
		ft_envadd_back(&last->env, ft_envnew(ft_strdup("?"), ft_itoa(retour
					% 256)));
	ft_free_exit(e_utils->current, e_utils->mini, e_utils->envp,
		e_utils->prompt);
	free(compare);
	return (retour % 256);
}

int	ft_exit_one_arg(t_exec_utils *e_utils, t_cmd *cmd)
{
	char	*compare;
	char	*tmp;
	char	*tmp2;

	compare = ft_ltoa(ft_atoll(cmd->args[1]));
	if (cmd->args[1][0] == '+')
	{
		tmp = ft_strndup(&cmd->args[1][0], 1);
		tmp2 = compare;
		compare = ft_strjoin_free(tmp, tmp2);
		free(tmp2);
	}
	if (!ft_is_num(cmd->args[1]))
		return (ft_print_exit(cmd->args[1]), ft_free_exit(e_utils->current,
				e_utils->mini, e_utils->envp, e_utils->prompt), free(compare),
			2);
	else if (ft_strcmp(compare, cmd->args[1]))
		return (ft_print_exit(cmd->args[1]), ft_free_exit(e_utils->current,
				e_utils->mini, e_utils->envp, e_utils->prompt), free(compare),
			2);
	else
		return (free(compare), ft_exit_one_ok(e_utils, cmd));
}

int	ft_exit(t_exec_utils *e_utils, t_cmd *cmd)
{
	static int	exit_status = 0;

	if (ft_tab_len(cmd->args) < 2)
	{
		ft_putendl_fd("exit", 1);
		return (ft_free_exit(e_utils->current, e_utils->mini, e_utils->envp,
				e_utils->prompt), exit(1), 0);
	}
	else if (ft_tab_len(cmd->args) > 2)
	{
		if (!ft_is_num(cmd->args[1]))
			return (ft_print_exit(cmd->args[1]), ft_free_exit(e_utils->current,
					e_utils->mini, e_utils->envp, e_utils->prompt), exit(2), 2);
		else
		{
			exit_status += 1;
			if (exit_status >= 1)
				return (ft_print_exit(NULL), 127);
			else
				return (ft_print_exit(NULL), 1);
		}
	}
	else
		exit(ft_exit_one_arg(e_utils, cmd));
}
