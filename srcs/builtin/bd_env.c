/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bd_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 10:09:52 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/11 12:25:22 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	put_env(t_var *lst, int fd)
{
	while (lst)
	{
		if (lst->key && lst->value)
		{
			ft_putstr_fd(lst->key, fd);
			ft_putstr_fd("=", fd);
			ft_putstr_fd(lst->value, fd);
			ft_putstr_fd("\n", fd);
		}
		lst = lst->next;
	}
}

int	bd_env(t_var *var_lst, char **cmde, int fd)
{
	if (!cmde || !cmde[0])
		return (SUCCESS);
	put_env(var_lst, fd);
	return (0);
}
