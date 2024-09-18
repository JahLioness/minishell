/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ede-cola <ede-cola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 13:40:27 by ede-cola          #+#    #+#             */
/*   Updated: 2024/09/18 11:40:51 by ede-cola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i = 1;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_exit(t_ast *root, t_mini **mini, char **envp, char *prompt)
{
	ft_clear_ast(root);
	ft_clear_lst(mini);
	ft_free_tab(envp);
	free(prompt);
}

void	ft_print_exit(char *str)
{
	ft_putendl_fd("exit", 1);
	ft_putstr_fd("minishell: exit: ", 2);
	if (str)
	{
		ft_putstr_fd(str, 2);
		ft_putendl_fd(": numeric argument required", 2);
	}
	else
		ft_putendl_fd("too many arguments", 2);
}

static int	ft_nbr_length_itoa(long long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n != 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_ltoa(long long nb)
{
	char	*ret;
	int		i;
	int		len;

	len = ft_nbr_length_itoa(nb);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	ret[len] = '\0';
	if (nb < 0)
	{
		i = 1;
		nb = -nb;
		ret[0] = '-';
	}
	else
		i = 0;
	while (len-- > i)
	{
		ret[len] = '0' + (nb % 10);
		nb /= 10;
	}
	return (ret);
}
