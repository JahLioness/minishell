/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:58:35 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/17 17:47:38 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(int fd, t_env **env)
{
	char	*pwd;
	t_env	*tmp;

	tmp = *env;
	if (!tmp)
	{
		pwd = getcwd(NULL, 0);
		ft_putendl_fd(pwd, fd);
		free(pwd);
	}
	else
	{
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, "PWD"))
			{
				ft_putendl_fd(tmp->value, fd);
				break ;
			}
			tmp = tmp->next;
		}
	}
	return (0);
}
