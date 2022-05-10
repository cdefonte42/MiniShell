/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 14:37:54 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/10 14:22:49 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

/* Cree un pipe et set both curr cmde et next cmde 's pipes */
int	ft_pipe_cmdes(t_cmde *c1, t_cmde *c2)
{
	int	pipefd[2];

	if (!c1 || !c2)
		return (SUCCESS);
	if (pipe(pipefd) == -1)
		return (perror("Pipe failed ft_pipe_cmdes"), FAILURE);
	c1->pipefd[w_end] = pipefd[w_end];
	c2->pipefd[r_end] = pipefd[r_end];
	return (SUCCESS);
}

int	ft_dup(t_cmde *cmde)
{
	if (cmde->pipefd[0] != 0 && dup2(cmde->pipefd[r_end], 0) == -1)
		return (perror("ft_dup to in failed"), FAILURE);
	if (cmde->pipefd[1] != 1 && dup2(cmde->pipefd[w_end], 1) == -1)
		return (perror("ft_dup to out failed"), FAILURE);
	return (SUCCESS);
}
