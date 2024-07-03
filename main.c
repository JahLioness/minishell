/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 18:26:41 by ede-cola          #+#    #+#             */
/*   Updated: 2024/07/03 18:26:47 by ede-cola         ###   ########.fr       */
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
