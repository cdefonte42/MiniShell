/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/18 16:23:01 by cdefonte          #+#    #+#             */
/*   Updated: 2022/02/23 15:13:01 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_execve(char **cmde_args, char **paths, t_cmde *cmde, char **env)
{
	if (execve(cmde_args[0], cmde_args, env) == -1)
	{
		ft_lstclear(&cmde);
		ft_free_tab(cmde_args);
		ft_free_tab(paths);
		exit(-1);
	}
}

/* Cmde 2 lit dans pipe, puis ecrit dans stdout, qui en fait est filename 
Return le pid du child, -1 sinon */
int	ft_exec_last(char **env, t_cmde *cmde, int (*f)(int *, char *))
{
	pid_t	pid;
	char	**paths;
	char	**cmde_args;

	pid = fork();
	if (pid == -1)
		return (perror("Error fork"), -1);
	if (pid == 0)
	{
		if (f && f(cmde->prev->pipe, cmde->filename) == -1)
			exit(ft_error_handler(cmde->filename, &cmde, file));
		ft_expansion(&cmde_args, &paths, env, cmde);
		ft_execve(cmde_args, paths, cmde, env);
	}
	return (pid);
}

/* Commamde 1 lit dans le fd et ecrit dans le pipe
Return le pid du child, -1 sinon */
int	ft_exec_first(char **env, t_cmde *cmde, int (*f)(int *, char *))
{
	pid_t	pid;
	char	**cmde_args;
	char	**paths;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (f && f(cmde->pipe, cmde->filename) == -1)
			exit(ft_error_handler(cmde->filename, &cmde, file));
		ft_expansion(&cmde_args, &paths, env, cmde);
		ft_execve(cmde_args, paths, cmde, env);
	}
	return (pid);
}

int	ft_exec_mid(char **env, t_cmde *cmde)
{
	pid_t	pid;
	char	**cmde_args;
	char	**paths;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (ft_redir_pipe_to(0, cmde->prev->pipe[in]) == -1
			|| ft_redir_pipe_to(1, cmde->pipe[out]) == -1)
			exit(ft_error_handler("ft_exec_mid", &cmde, command));
		if (close(cmde->pipe[in]) == -1)
			exit(ft_error_handler("ft_exec_mid", &cmde, file));
		ft_expansion(&cmde_args, &paths, env, cmde);
		ft_execve(cmde_args, paths, cmde, env);
	}
	return (pid);
}

int	ft_wait_cmde(pid_t pid)
{
	int		status;
	pid_t	w;

	status = 0;
	w = waitpid(pid, &status, 0);
	if (w == -1)
		return (perror("waipid failed"), -1);
	if (WIFEXITED(status) && status != 0)
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status) && status != 0)
		return (WTERMSIG(status) + 128);
	return (0);
}
