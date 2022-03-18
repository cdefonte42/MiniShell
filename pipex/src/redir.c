/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:29:38 by cdefonte          #+#    #+#             */
/*   Updated: 2022/02/18 12:02:07 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_redir_pipe_to(int fd, int pipe)
{
	if (dup2(pipe, fd) == -1)
		return (-1);
	if (close(pipe) == -1)
		return (-1);
	return (0);
}

/* Duplicate / redirect stdout to filename 
Warning: Correct access needed
Return 0 on success, -1 on bad syscall*/
int	ft_redir_stdout_to_file(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, \
S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		return (-1);
	if (dup2(fd, 1) == -1)
		return (-1);
	if (close(fd) == -1)
		return (-1);
	return (0);
}

/* Duplicate / redirect filename's fd to stdin (fd 0) 
Warning: Correct access needed
Return 0 on success, -1 on bad syscall*/
int	ft_redir_file_to_stdin(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	if (dup2(fd, 0) == -1)
		return (-1);
	if (close(fd) == -1)
		return (-1);
	return (0);
}

int	ft_file2(int *pipefd, char *filename)
{
	if (access(filename, F_OK) == 0 && access(filename, R_OK) != 0)
		return (-1);
	if (ft_redir_stdout_to_file(filename) == -1)
		return (-1);
	if (close(pipefd[1]) == -1)
		return (-1);
	if (ft_redir_pipe_to(0, pipefd[0]) == -1)
		return (-1);
	return (0);
}

int	ft_file1(int *pipefd, char *filename)
{
	errno = 0;
	if (access(filename, F_OK) != 0 || access(filename, R_OK) != 0)
		return (-1);
	if (ft_redir_file_to_stdin(filename) == -1)
		return (-1);
	if (close(pipefd[0]) == -1)
		return (-1);
	if (ft_redir_pipe_to(1, pipefd[1]) == -1)
		return (-1);
	return (0);
}
