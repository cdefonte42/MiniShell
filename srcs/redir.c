/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:59:27 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/05 10:45:55 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

extern int	g_status;

int	ft_token_isredir(t_token_type type)
{
	if (type == redirin || type == redirout
		|| type == redirapp || type == heredoc)
		return (1);
	return (0);
}

/*
heredoc = , O_CREAT | O_WRONLY | O_TRUNC, 00644		=> Pour readline
heredoc = O_RDONLY, 00644							=> pour le lire
out		= , O_WRONLY | O_CREAT | O_TRUNC = 01101
 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH = 00644
in		= , O_RDONLY
app		= O_APPEND | O_CREAT | O_WRONLY = 01011 (MAC) OU 02101 (LINUX)
*/
int	ft_open(int *fd, char *pathname, int flags, int mode)
{
	int	new_fd;

	if (*fd != 1 && *fd != 0 && close(*fd) == -1)
		perror("closing fd failed ft_open");
	errno = 0;
	new_fd = open(pathname, flags, mode);
	if (new_fd == -1)
		return (FAILURE);
	*fd = new_fd;
	return (SUCCESS);
}

int	ft_open_hd(int *fd, char *pathname, int flags, int mode)
{
	int	new_fd;

	if (*fd != 1 && *fd != 0 && close(*fd) == -1)
		perror("closing fd failed ft_open");
	errno = 0;
	new_fd = open(pathname, flags, mode);
	if (new_fd == -1)
		return (FAILURE);
	*fd = new_fd;
	if (unlink(pathname) == -1)
		return (perror("unlink heredoc"), FAILURE);
	return (SUCCESS);
}

void	signal_hdtest(int sig)
{
	g_status = 128 + sig;
	if (sig == SIGINT)
	{
		ft_putstr_fd("SIGINT catched heredoc\n", 2);
	}
}

int	ft_testhd(t_cmde *cmde, char *delimiter, int quoted, t_var *vars)
{
	//if (!ft_open_hd(&(cmde->pipefd[r_end]), cmde->hdfile, 0, 00644) == FAILURE)
	int		hd_pipe[2];
	char			*line;

	(void)quoted;
	(void)vars;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &signal_hdtest);
	if (pipe(hd_pipe) == -1)
		return (FAILURE);
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
		ft_putstr_fd(line, hd_pipe[w_end]);
		free(line);
		line = NULL;
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (line == NULL && g_status != 130)
			ft_error("warning: here-document delimited by \
en-of-file. Wanted", delimiter);
	}
	if (close(hd_pipe[w_end]) == -1)
		perror("closing h_pipe w_end failed\n");
	free(line);
	line = NULL;
	if (cmde->pipefd[r_end] != 0 && close(cmde->pipefd[r_end]) == -1)
		perror("closin pipefd r_end cmde faied\n");
	cmde->pipefd[r_end] = hd_pipe[r_end];
	return (SUCCESS);
}

int	switch_redir(t_cmde *cmde, char *file, int type)
{
	if (type == redirin)
		if (ft_open(&(cmde->pipefd[r_end]), file, O_RDONLY, 0) == FAILURE)
			return (ft_perror(file, NULL), FAILURE);
	if (type == redirout)
		if (ft_open(&(cmde->pipefd[w_end]), file, 01101, 00644) == FAILURE)
			return (ft_perror(file, NULL), FAILURE);
	if (type == redirapp)
		if (ft_open(&(cmde->pipefd[w_end]), file, 02101, 00644) == FAILURE)
			return (ft_perror(file, NULL), FAILURE);
	if (type == heredoc)
		if (ft_testhd(cmde, file, 0, NULL) == FAILURE)
			return (ft_perror(cmde->hdfile, NULL), FAILURE);
	return (SUCCESS);
}

int	ft_redir(t_cmde *cmde)
{
	t_token	*head_token;
	char	*file;

	if (!cmde || !cmde->cmde_line)
		return (SUCCESS);
	head_token = cmde->cmde_line;
	while (head_token)
	{
		if (ft_token_isredir(head_token->type))
		{
			file = head_token->next->str;
			if (file && *file == 0)
				return (ft_error(file, "ambiguous redirect\n"), FAILURE);
			else if (switch_redir(cmde, file, head_token->type) == FAILURE)
				return (FAILURE);
		}
		head_token = head_token->next;
	}
	return (SUCCESS);
}
