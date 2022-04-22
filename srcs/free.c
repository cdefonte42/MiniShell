/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 19:16:14 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/21 17:23:54 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

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
	ft_varlst_clear(msh->vars);
	ft_cmdelst_clear(msh->cmde_lst);
}
