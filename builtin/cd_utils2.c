/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:55:46 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/23 17:02:41 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_dir(char *new_dir, char *value)
{
	char	*tmp_dir;
	char	*tmp_new;

	tmp_dir = ft_strdup_free(new_dir);
	new_dir = ft_strchr(tmp_dir, '/');
	tmp_new = ft_strdup(new_dir);
	free(tmp_dir);
	new_dir = ft_strjoin(value, tmp_new);
	free(tmp_new);
	return (new_dir);
}
