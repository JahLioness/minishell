/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:44:49 by ede-cola          #+#    #+#             */
/*   Updated: 2024/05/28 12:50:07 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_mini	*mini_lst;

	(void)av;
	mini_lst = NULL;
	if (ac != 1)
	{
		ft_putstr_fd("Error: minishell does not take any arguments\n", 2);
		return (1);
	}
	ft_prompt(&mini_lst, envp);
	return (0);
}
