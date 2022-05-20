/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 11:39:43 by mbraets           #+#    #+#             */
/*   Updated: 2022/05/20 14:57:42 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char **av)
{
	int	i;
	int	option;

	i = 0;
	option = 0;
	if (!av || !av[0] || !(*av))
		return (0);
	if ((*av)[i] == '-')
	{
		i++;
		while ((*av)[i])
		{
			if ((*av)[i] == 'n')
				option = true;
			else
				return (false);
			i++;
		}
	}
	return (option);
}

int	minishell_echo(t_minishell *msh, char **av, int fdout)
{
	int	option;
	int	i;

	(void) msh;
	i = 1;
	if (!av)
		return (0);
	option = check_option(av + i);
	while (check_option(av + i))
		i++;
	while (av && av[i])
	{
		ft_putstr_fd(av[i++], fdout);
		if (av[i] != NULL)
			ft_putchar_fd(' ', fdout);
	}
	if (option == false)
		ft_putchar_fd('\n', fdout);
	return (0);
}
