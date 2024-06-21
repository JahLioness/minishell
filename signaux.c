/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:06:44 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/21 19:30:10 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_handler_ctrl_c(int signum, siginfo_t *info, void *context)
// {
// 	(void)context;
// 	if (signum == SIGINT && info->si_pid > 0)
// 	{
// 		ft_putendl_fd("", 1);
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// 	return ;
// }

// void	ft_get_signal(void)
// {
// 	struct sigaction	act;
// 	struct sigaction	oldact;

// 	ft_bzero(&oldact, sizeof(oldact));
// 	act.sa_flags = SA_SIGINFO;
// 	act.sa_sigaction = &ft_handler_ctrl_c;
// 	oldact.sa_handler = SIG_IGN;
// 	sigemptyset(&act.sa_mask);
// 	if (sigaction(SIGINT, &act, NULL) < 0)
// 		return ;
// 	if (sigaction(SIGQUIT, &oldact, NULL) < 0)
// 		return ;
// }

void	ft_handler_sigquit(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGQUIT && info->si_pid == 0)
	{
		printf("Quit (core dumped)\n");
		exit (EXIT_FAILURE);
	}
}

void	ft_get_signal_cmd(void)
{
	struct sigaction	act;
	// struct sigaction	oldact;

	// ft_bzero(&oldact, sizeof(oldact));
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = &ft_handler_sigquit;
	// oldact.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGQUIT, &act, NULL) < 0)
		return ;
	// if (sigaction(SIGINT, &oldact, NULL) < 0)
	// 	return ;
}

// void	ft_handler(int signum, siginfo_t *info, void *context)
// {
// 	struct sigaction	oldact;

// 	(void)context;
// 	ft_bzero(&oldact, sizeof(oldact));
// 	oldact.sa_handler = SIG_IGN;
// 	if (signum == SIGINT && info->si_pid > 0)
// 	{
// 		printf("\n");
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// 	else if (signum == SIGQUIT && info->si_pid == 0)
// 	{
// 		printf("Quit (core dumped)\n");
// 		exit (EXIT_FAILURE);
// 	}
// 	if (signum == SIGQUIT && info->si_pid > 0)
// 	{
// 		printf("here\n");
// 		if (sigaction(SIGQUIT, &oldact, NULL) < 0)
// 			return ;
// 	}
// 	return ;
// }

// void	ft_get_signal(void)
// {
// 	struct sigaction	act;

// 	act.sa_flags = SA_RESETHAND;
// 	act.sa_sigaction = &ft_handler;
// 	sigemptyset(&act.sa_mask);
// 	if (sigaction(SIGINT, &act, NULL) < 0)
// 		return ;
// 	if (sigaction(SIGQUIT, &act, NULL) < 0)
// 		return ;
// }

void	ft_handler(int signum, siginfo_t *info, void *context)
{
	(void)context;
	if (signum == SIGINT && info->si_pid > 0)
	{
		ft_putendl_fd("", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	g_sig = signum;
}

void	ft_get_signal(void)
{
	struct sigaction	act;
	struct sigaction	oldact;

	ft_bzero(&oldact, sizeof(oldact));
	ft_bzero(&act, sizeof(oldact));
	oldact.sa_handler = SIG_IGN;
	act.sa_sigaction = &ft_handler;
	act.sa_flags = SA_SIGINFO;
	if (sigaction(SIGINT, &act, NULL) < 0)
		return ;
	if (sigaction(SIGQUIT, &oldact, NULL) < 0)
		return ;
}