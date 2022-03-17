/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/04 07:48:49 by cdefonte          #+#    #+#             */
/*   Updated: 2022/02/23 12:12:51 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv)
{
	extern char	**environ;
	int			exit_stat;
	t_cmde		*cmde;
	t_cmde		*first;

	errno = 0;
	cmde = NULL;
	if (argc < 5)
		return (ft_putstr_fd("Need at least 5 arguments\n", 2), 1);
	if (ft_init_cmdes(&cmde, argc, argv) == -1)
		return (perror("init_cmdes failed"), 1);
	first = cmde;
	if (ft_pipex_first(environ, &cmde) == -1)
		return (1);
	if (ft_pipex_mid(environ, &cmde, argc) == -1)
		return (1);
	exit_stat = ft_pipex_last(environ, &cmde);
	ft_lstclear(&first);
	return (exit_stat);
}
