/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 19:16:14 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/29 16:11:34 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

extern int	g_status;

void	minishell_free_rawcmd(t_minishell *msh)
{
	int	i;

	i = 0;
	if (msh->raw_cmd)
	{
		while (msh->raw_cmd[i] != NULL)
		{
			free(msh->raw_cmd[i]);
			i++;
		}
		free(msh->raw_cmd);
	}
}

/* Free tous les elements et leur CONTENU */
void	ft_msh_clear(t_minishell *msh)
{
	t_cmde	*cmde;

	cmde = msh->cmde_lst;
	while (cmde)
	{
		if (cmde->pipefd[0] != 0 && close(cmde->pipefd[0]) == -1)
			perror("mshclear closin r_end pipe failed");
		if (cmde->pipefd[1] != 1 && close(cmde->pipefd[1]) == -1)
			perror("mshclear closin w_end pipe failed");
		cmde = cmde->next;
	}
	if (msh->vars)
		ft_varlst_clear(msh->vars);
	if (msh->cmde_lst)
		ft_cmdelst_clear(msh->cmde_lst);
}
