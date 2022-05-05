/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/30 21:48:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/05 13:03:48 by cdefonte         ###   ########.fr       */
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
		close(0);
}

int	isprelastchr(char c, char *str)
{
	int		i;
	int		len;

	i = 0;
	if (!str)
		return (0);
	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == c)
		return (1);
	return (0);
}

int	ft_heredoc_input(char *delimiter, int fd, int quoted, t_var *vars)
{
	char			*line;

	line = readline("> ");
	while (line && g_status != 130 && ft_strcmp(line, delimiter) != 0)
	{
		if (quoted == nil && expand_hdstr(&line, vars) == FAILURE)
			return (free(line), g_status = 12, FAILURE);
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
		line = NULL;
		line = readline("> ");
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
	int		ret_stat;

	ret_stat = 0;
	pid = fork();
	if (pid == -1)
		return (-1);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		g_status = 0;
		if (ft_heredoc(msh->vars, cmde, delimiter) == -1)
			g_status = 12;
		ft_msh_clear(msh);
		exit(g_status);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ret_stat = WEXITSTATUS(status);
	if (ret_stat == 12)
		return (-1);
	return (ret_stat);
}
