/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 19:42:50 by andjenna          #+#    #+#             */
/*   Updated: 2024/08/24 20:43:37 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handler_heredoc(int signum, siginfo_t *info, void *context)
{
	(void)context;
	g_sig = signum;
	kill(info->si_pid, SIGINT);
}

void	ft_get_signal_heredoc(void)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	act.sa_sigaction = &ft_handler_heredoc;
	signal(SIGQUIT, SIG_IGN);
	sigaction(SIGINT, &act, NULL);
}
