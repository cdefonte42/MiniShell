/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 11:39:43 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/27 15:38:36 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char **av)
{
	int	i;
	int	option;

	i = 0;
	option = 0;
	if (!av || !av[0] || !av[1])
		return (0);
	if (av[1][i] == '-')
	{
		i++;
		while (av[1][i])
		{
			if (av[1][i] == 'n')
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
	option = check_option(av);
	if (option == true)
		i = 2;
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
