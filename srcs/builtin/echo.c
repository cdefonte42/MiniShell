/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 11:39:43 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/23 14:23:57 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char **av)
{
	int	i;
	int	option;

	i = 0;
	option = 0;
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

int	minishell_echo(t_minishell *msh, char **av)
{
	int	option;
	int	i;

	(void) msh;
	av = av + 1;
	i = 0;
	option = check_option(av + i);
	while (check_option(av + i))
		i++;
	while (av[i])
	{
		ft_putstr_fd(av[i++], 1);
		if (av[i] != NULL)
			ft_putchar_fd(' ', 1);
	}
	if (option == false)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}
