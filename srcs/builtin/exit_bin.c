/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 12:38:13 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/20 16:45:09 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	ft_exitbin(t_minishell *msh, char **cmd)
{
	int	ret_stat;

	ret_stat = g_status;
	if (cmd[1] != NULL)
	{
		if (ft_stris(cmd[1], ft_isdigit) && ft_sli(cmd[1]) < 19)
			ret_stat = ft_atoi(cmd[1]);
		else
		{
			msh->loop = 0;
			ft_putendl_fd("exit", 1);
			return (ft_error(cmd[1], "numeric argument required"), 2);
		}
		if (ft_stris(cmd[1], ft_isdigit) && ft_sli(cmd[1]) < 19 && cmd[2])
		{
			ft_putendl_fd("exit", 1);
			return (ft_error("too many arguments", NULL), 1);
		}
	}
	ft_putendl_fd("exit", 1);
	msh->loop = 0;
	return (ret_stat);
}
