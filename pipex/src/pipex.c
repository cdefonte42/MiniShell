/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 16:15:00 by cdefonte          #+#    #+#             */
/*   Updated: 2022/02/23 12:38:41 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_pipex_first(char **environ, t_cmde **list)
{
	t_cmde	*cmde;

	cmde = *list;
	if (pipe(cmde->pipe) == -1)
		return (perror("pipe fct"), 1);
	cmde->pid = ft_exec_first(environ, cmde, &ft_file1);
	if (cmde->pid <= 0)
		return (-1);
	if (close(cmde->pipe[out]) == -1)
		return (perror("close pipe first out exec"), -1);
	return (0);
}

int	ft_pipex_mid(char **environ, t_cmde **list, int argc)
{
	int		i;
	t_cmde	*cmde;

	cmde = *list;
	i = 0;
	while (i < argc - 5)
	{
		cmde = cmde->next;
		if (pipe(cmde->pipe) == -1)
			return (perror("pipe fct"), 1);
		cmde->pid = ft_exec_mid(environ, cmde);
		if (cmde->pid <= 0)
			return (-1);
		if (close(cmde->prev->pipe[in]) == -1)
			return (perror("close prev pipe mid exec"), -1);
		if (close(cmde->pipe[out]) == -1)
			return (perror("close pipe mid exec"), -1);
		i++;
	}
	*list = cmde->next;
	return (0);
}

int	ft_pipex_last(char **environ, t_cmde **list)
{
	int		exit_stat;
	t_cmde	*cmde;

	cmde = *list;
	if (pipe(cmde->pipe) == -1)
		return (perror("pipe fct"), 1);
	cmde->pid = ft_exec_last(environ, cmde, &ft_file2);
	if (cmde->pid <= 0)
		return (-1);
	if (close(cmde->prev->pipe[in]) == -1)
		return (perror("close prev pipe last exec"), -1);
	if (close(cmde->pipe[out]) == -1)
		return (perror("close prev pipe last exec"), -1);
	exit_stat = ft_wait_cmde(cmde->pid);
	if (close(cmde->pipe[in]) == -1)
		return (perror("close pipe last in exec"), -1);
	if (ft_wait_cmde(-1) == -1)
		return (-1);
	return (exit_stat);
}
