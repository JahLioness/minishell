/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 11:03:48 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/18 15:19:37 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_prompt(t_mini **mini_lst, char **envp)
{
	char	*line;
	char	*prompt;
	t_mini	*last;
	t_env	*e_status;

	*mini_lst = ft_mini_init();
	if (!*mini_lst || !mini_lst)
		exit(1);
	if (envp && *envp)
		(*mini_lst)->env = ft_get_env(envp);
	else
		(*mini_lst)->env = NULL;
	prompt = ft_get_prompt((*mini_lst)->env);
	if (!prompt)
		exit (1);
	while (1)
	{
		ft_get_signal();
		line = readline(prompt);
		if(g_sig == SIGINT)
		{
			last = ft_minilast(*mini_lst);
			e_status = ft_get_exit_status(&last->env);
			ft_change_exit_status(e_status, ft_itoa(130));
			g_sig = 0;
		}
		if (!line)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*line)
			add_history(line);
		if (ft_strlen(line) > 0)
		{
			ft_miniadd_back(mini_lst, ft_new_mini(line, mini_lst));
			if (!ft_minilast(*mini_lst)->error)
				ft_exec_token(mini_lst, prompt);
		}
		if (!ft_strncmp(line, "cd", 2))
		{
			last = ft_minilast(*mini_lst);
			free(prompt);
			prompt = ft_get_prompt(last->env);
		}
		free(line);
	}
	free(prompt);
	ft_print_lst(*mini_lst);
	rl_clear_history();
	ft_clear_lst(mini_lst);
	exit(0);
}
