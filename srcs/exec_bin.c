/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:53:12 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/11 11:43:59 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

extern int	g_status;

int	set_g_status(void)
{
	if (errno == 12 || errno == 5)
		return (-1);
	else
		return (1);
}

int	launch_bin(t_minishell *msh, t_cmde *cmde, char **raw_cmd)
{
	int	ret_stat;

	ret_stat = 0;
	if ((ft_strcmp(raw_cmd[0], "exit") == 0))
		ret_stat = ft_exitbin(msh, raw_cmd);
	else if ((ft_strcmp(raw_cmd[0], "cd") == 0))
		ret_stat = ft_cd(&(msh->vars), raw_cmd);
	else if ((ft_strcmp(raw_cmd[0], "pwd") == 0))
		ret_stat = ft_pwd(cmde->pipefd[w_end]);
	else if ((ft_strcmp(raw_cmd[0], "export") == 0))
		ret_stat = ft_export(&(msh->vars), raw_cmd, cmde->pipefd[w_end]);
	else if ((ft_strcmp(raw_cmd[0], "env") == 0))
		ret_stat = bd_env(msh->vars, raw_cmd, cmde->pipefd[w_end]);
	else if ((ft_strcmp(raw_cmd[0], "unset") == 0))
		ret_stat = ft_unset(&(msh->vars), raw_cmd);
	else if ((ft_strcmp(raw_cmd[0], "echo") == 0))
		ret_stat = minishell_echo(msh, raw_cmd, cmde->pipefd[w_end]);
	return (ret_stat);
}

int	ft_exec_bin(t_minishell *msh, t_cmde *cmde)
{
	char	**raw_cmd;
	int		ret_stat;

	ret_stat = 0;
	errno = 0;
	if (!cmde || !cmde->cmde_line)
		return (ret_stat);
	if (ft_redir(cmde) == FAILURE)
		return (set_g_status());
	if (cmde->name_token)
	{
		raw_cmd = ft_lst_to_char(cmde->cmde_line);
		if (!raw_cmd)
			return (-1);
		ret_stat = launch_bin(msh, cmde, raw_cmd);
		free(raw_cmd);
		raw_cmd = NULL;
	}
	return (ret_stat);
}
