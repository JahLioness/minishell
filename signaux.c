/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:06:44 by ede-cola          #+#    #+#             */
/*   Updated: 2024/08/24 19:42:02 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig = 0;

void	ft_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	g_sig = signum;
	if (signum == SIGINT && info->si_pid > 0)
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signum == SIGINT && info->si_pid == 0)
		printf("\n");
}

void	ft_get_signal(void)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &ft_handler;
	if (sigaction(SIGINT, &act, NULL) == -1)
		exit (EXIT_FAILURE);
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &act, NULL) == -1)
		exit (EXIT_FAILURE);
}

void	ft_handler_sigquit(int signum)
{
	g_sig = signum;
	ft_putendl_fd("Quit (core dumped)", 2);
}

void	ft_get_signal_cmd(void)
{
	signal(SIGQUIT, ft_handler_sigquit);
}
