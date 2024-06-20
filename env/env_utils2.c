/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:32:25 by ede-cola          #+#    #+#             */
/*   Updated: 2024/06/17 14:15:26 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_get_exit_status(t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "?"))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	ft_change_exit_status(t_env *status, char *value)
{
	if (status->value)
		free(status->value);
	status->value = value;
}
