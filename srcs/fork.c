/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:50:33 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/11 11:41:37 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

extern int	g_status;

void	ft_exit_child(t_child child, t_minishell *msh, t_cmde *cmde, bool err)
{
	if (err)
	{
		if (errno != 2)
			ft_perror(cmde->name_token->str, NULL);
		else
			ft_error(cmde->name_token->str, "command not found");
	}
	free(child.argv);
	free(child.pathname);
	ft_free_tabtab(child.envp);
	ft_msh_clear(msh);
	if (errno == 13)
		exit(126);
	else if (errno == 2 || errno == 36)
		exit(127);
	else if (errno == 5)
		exit(254);
	exit(EXIT_FAILURE);
}

void	pre_fork(t_minishell *msh, t_cmde *cmde, t_child *child)
{
	if (ft_redir(cmde) == FAILURE)
		ft_exit_child(*child, msh, cmde, false);
	if (ft_dup(cmde) == FAILURE)
		ft_exit_child(*child, msh, cmde, true);
	if (ft_isbin(cmde->name_token->str))
	{
		g_status = ft_exec_bin(msh, cmde);
		ft_msh_clear(msh);
		exit(g_status);
	}
	child->argv = ft_lst_to_char(cmde->cmde_line);
	if (!child->argv)
		ft_exit_child(*child, msh, cmde, true);
	child->envp = ft_varlst_tochar(msh->vars);
	if (!child->envp)
		ft_exit_child(*child, msh, cmde, true);
	child->pathname = check_permission(msh, child->argv[0]);
	if (!child->pathname)
		ft_exit_child(*child, msh, cmde, true);
	if (is_dir(child->pathname))
		ft_exit_child(*child, msh, cmde, true);
	if (cmde->next && close(cmde->next->pipefd[r_end]) == -1)
		perror("_2_mope closing in ds fork");
}

int	ft_fork(t_minishell *msh, t_cmde *cmde)
{
	t_child	child;

	cmde->pid = fork();
	if (cmde->pid == -1)
		return (perror("fork failed"), FAILURE);
	ft_memset(&child, 0, sizeof(t_child));
	errno = 0;
	if (cmde->pid == 0)
	{
		pre_fork(msh, cmde, &child);
		execve(child.pathname, child.argv, child.envp);
		ft_exit_child(child, msh, cmde, true);
		exit(FAILURE);
	}
	if (cmde->pipefd[w_end] != 1 && close(cmde->pipefd[w_end]) == -1)
		perror("closing pipeout ft_forkout failed");
	if (cmde->pipefd[r_end] != 0 && close(cmde->pipefd[r_end]) == -1)
		perror("closing pipin ft_forkout failed");
	return (SUCCESS);
}

int	ft_exec(t_minishell *msh, t_cmde *cmde)
{
	if (!cmde || !cmde->cmde_line)
		return (SUCCESS);
	if ((cmde->next || cmde->prev) || !cmde->name_token || !ft_isbin(cmde->name_token->str))
	{
		if (ft_pipe_cmdes(cmde, cmde->next) == FAILURE)
			return (FAILURE);
		if (ft_fork(msh, cmde) == FAILURE)
			return (perror("ft_fork failed ds ft_exec"), FAILURE);
	}
	else
		g_status = ft_exec_bin(msh, cmde);
	if (g_status == -1)
		return (FAILURE);
	return (SUCCESS);
}
