/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:59:27 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/22 19:22:41 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

int	ft_pipe_cmdes(t_cmde *c1, t_cmde *c2)
{
	int	pipefd[2];

	if (!c1 || !c2)
		return (SUCCESS);
	if (pipe(pipefd) == -1)
		return (perror("Pipe failed ft_pipe_cmdes"), FAILURE);
	c1->pipefd[out] = pipefd[out];
	c2->pipefd[in] = pipefd[in];
	if (close(pipefd[in]) == -1 || close(pipefd[out]) == -1)
		return (perror("closing pipes ft_pipe_cmdes"), FAILURE);
	return (SUCCESS);
}

int	ft_token_isredir(t_token_type type)
{
	if (type == redirin || type == redirout || type == redirapp)
		return (1);
	return (0);
}
/*
heredoc = , O_CREAT | O_WRONLY | O_TRUNC, 00644		=> Pour readline
heredoc = O_RDONLY, 00644							=> pour le lire
out		= , O_WRONLY | O_CREAT | O_TRUNC = 01101
 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH = 00644
in		= , O_RDONLY
app		= O_APPEND | O_CREAT | O_WRONLY = 02101
*/

int	ft_open(int *fd, char *pathname, int flags, int mode)
{
	*fd = open(pathname, flags, mode);
	if (*fd == -1)
		return (ft_perror(NULL, pathname), FAILURE);
	return (SUCCESS);

}
int	ft_redir(t_cmde *cmde)
{
	t_token *head_token;
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
			else
			{
				if (head_token->type == redirin)
					if (ft_open(&(cmde->fdin), file, O_RDONLY, 0) == FAILURE)
						return (FAILURE);
				if (head_token->type == redirout)
					if (ft_open(&(cmde->fdout), file, 01101, 00644) == FAILURE)
						return (FAILURE);			
				if (head_token->type == redirapp)
					if (ft_open(&(cmde->fdout), file, 02101, 00644) == FAILURE)
						return (FAILURE);
			}
		}
		head_token = head_token->next;
	}
	return (SUCCESS);
}
