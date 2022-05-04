/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/30 21:48:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/04 13:08:28 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
#include "minishell.h"
#include "libft.h"

extern int	g_status;

void	signal_hd(int sig)
{
	g_status = 128 + sig;
	if (sig == SIGINT)
	{
		//ft_putstr_fd("SIGINT catched heredoc\n", 2);
		close(0);
	}
}

void	ignore_sig(int q)
{
	(void) q;
}

int	ft_heredoc_input(char *delimiter, int fd, int quoted, t_var *vars)
{
	char			*line;

	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	if (!line)
		ft_putstr_fd("\n", 2);
	while (line && g_status != 130)
	{
		if (ft_strlen(line) > ft_strlen(delimiter) && \
		!ft_strncmp(line, delimiter, ft_strlen(line) - 1))
			break ;
		if (quoted == nil && expand_hdstr(&line, vars) == FAILURE)
			return (free(line), perror("expand_hdstr failed"), FAILURE);
		ft_putstr_fd(line, fd);
		free(line);
		line = NULL;
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (line == NULL && g_status != 130)
			ft_error("warning: here-document delimited by \
en-of-file. Wanted", delimiter);
	}
	free(line);
	line = NULL;
	return (SUCCESS);
}

int	ft_heredoc(t_var *vars, t_cmde *cmde, char **delimiter)
{	
	int				fd;
	t_quote_type	quoted;

	signal(SIGINT, &signal_hd);
	signal(SIGQUIT, SIG_IGN);
	quoted = msh_isquoted(*delimiter);
	if (remove_quote(delimiter) == FAILURE)
		return (perror("ft_heredoc remove auote failed"), -1);
	fd = open(cmde->hdfile, O_CREAT | O_WRONLY | O_TRUNC, 00644);
	if (fd == -1)
		return (perror("ft_heredoc opening fd failed"), 1);
	ft_heredoc_input(*delimiter, fd, quoted, vars);
	if (close(fd) == -1)
		perror("ft_heredoc close fd failed");
	return (g_status);
}

int	heredoc_fork(t_minishell *msh, t_cmde *cmde, char **delimiter)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (FAILURE);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		if (ft_heredoc(msh->vars, cmde, delimiter) == -1)
			g_status = 12;
		ft_msh_clear(msh);
		exit(g_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		if (g_status == 12)
			return (FAILURE);
	}
	return (SUCCESS);
}
