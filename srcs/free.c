/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 19:16:14 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/07 12:31:59 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"
void	minishell_free_env(t_minishell *msh)
{
	int	i;
	int	j;

	i = 0;
	if (msh->env)
	{
		while (msh->env[i] != NULL)
		{
			j = 0;
			while (msh->env[i][j] != NULL)
			{
				free(msh->env[i][j]);
				j++;
			}
				free(msh->env[i][j]);
			free(msh->env[i]);
			i++;
		}
			free(msh->env[i]);
		free(msh->env);
	}
	if (msh->vars != NULL)
		ft_lst_clear(msh->vars);
}

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
