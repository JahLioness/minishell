/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:59:48 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/04 16:00:05 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(char *key, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;
	int		i;

	if (!env || !*env)
		return ;
	tmp = *env;
	prev = tmp;
	i = 0;
	while (tmp)
	{
		if (!strncmp(tmp->key, key, ft_strlen(key)))
		{
			if (i == 0)
				*env = tmp->next;
			else
				prev->next = tmp->next;
			ft_env_delone(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
		i++;
	}
}
