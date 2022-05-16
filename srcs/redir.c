/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:59:27 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/11 12:06:59 by cdefonte         ###   ########.fr       */
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

int	switch_redir(t_cmde *cmde, char *file, int type)
{
	if (type == redirin)
		if (!ft_open(&(cmde->pipefd[r_end]), file, O_RDONLY, 0))
			return (ft_perror(file, NULL), FAILURE);
	if (type == redirout)
		if (!ft_open(&(cmde->pipefd[w_end]), file, 01101, 00644))
			return (ft_perror(file, NULL), FAILURE);
	if (type == redirapp)
		if (!ft_open(&(cmde->pipefd[w_end]), file, 02101, 00644))
			return (ft_perror(file, NULL), FAILURE);
	if (type == heredoc)
		if (!ft_open_hd(&(cmde->pipefd[r_end]), cmde->hdfile, 0, 00644))
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
			if (head_token->next == NULL && head_token->type != heredoc)
				return (ft_error(NULL, "ambiguous redirect\n"), errno = 0, 0);
			file = head_token->next->str;
			if (file && *file == 0 && head_token->type != heredoc)
				return (ft_error(file, "ambiguous redirect\n"), errno = 0, 0);
			else if (switch_redir(cmde, file, head_token->type) == FAILURE)
				return (errno = 0, FAILURE);
		}
		head_token = head_token->next;
	}
	return (SUCCESS);
}
