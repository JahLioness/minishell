/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 14:10:03 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 17:21:05 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_prompt_user(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "USER", 7))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static char	*ft_get_prompt_post(t_env *env)
{
	t_env	*tmp;
	char	**split;
	char	*ret;

	tmp = env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "SESSION_MANAGER", 16))
		{
			split = ft_split(ft_strchr(tmp->value, '/') + 1, '.');
			if (!split)
				return (NULL);
			ret = ft_strdup(split[0]);
			if (!ret)
				return (ft_free_tab(split), NULL);
			ft_free_tab(split);
			return (ret);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

static char	*ft_get_ret(char *ret, char *user, char *post)
{
	char	*cwd;

	ret = ft_strjoin(user, "@");
	ret = ft_strjoin_free(ret, post);
	ret = ft_strjoin_free(ret, ":");
	cwd = getcwd(NULL, 0);
	if (cwd && user && !ft_strncmp(cwd, "/home/", ft_strlen("/home/")))
	{
		ret = ft_strjoin_free(ret, "~");
		ret = ft_strjoin_free(ret, ft_strchr(cwd, '/') + 6 + ft_strlen(user));
		free(cwd);
	}
	else
	{
		if (cwd)
			ret = ft_strjoin_free(ret, cwd);
		free(cwd);
	}
	return (ret);
}

char	*ft_get_prompt(t_env *env)
{
	char	*user;
	char	*post;
	char	*cwd;
	char	*ret;

	ret = NULL;
	user = ft_get_prompt_user(env);
	post = ft_get_prompt_post(env);
	if (!env || !user || !post)
	{
		ret = ft_strdup("~");
		cwd = getcwd(NULL, 0);
		if (cwd && ft_strchr(ft_strchr(cwd, '/') + 6, '/'))
			ret = ft_strjoin_free(ret, ft_strchr(ft_strchr(cwd, '/') + 6, '/'));
		ret = ft_strjoin_free(ret, "$ ");
		return (free(cwd), ret);
	}
	ret = ft_get_ret(ret, user, post);
	ret = ft_strjoin_free(ret, "$ ");
	return (free(post), ret);
}
