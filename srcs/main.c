/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/10 16:20:39 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

int	g_status;

void	signal_handler(int signalid)
{
	if (signalid == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 130;
	}
	if (signalid == SIGQUIT || signalid == SIGTSTP)
		write(1, "\b\b  \b\b", 6);
}

int	launch_exec(t_minishell *msh)
{
	t_cmde	*curr_cmde;
	t_cmde	*prev;

	curr_cmde = msh->cmde_lst;
	while (curr_cmde)
	{
		ft_expand_tokens(curr_cmde, msh->vars);
		if (ft_tokenlst_iteri_s(curr_cmde->cmde_line, &remove_quote) == FAILURE
			|| ft_exec(msh, curr_cmde) == FAILURE)
			return (FAILURE);
		if (curr_cmde->next == NULL && curr_cmde->pid != -1)
		{
			g_status = ft_wait_cmde(curr_cmde->pid, 0);
			prev = curr_cmde->prev;
			while (prev)
			{
				ft_wait_cmde(prev->pid, 0);
				prev = prev->prev;
			}
		}
		curr_cmde = curr_cmde->next;
	}
	ft_cmdelst_clear(msh->cmde_lst);
	msh->cmde_lst = NULL;
	return (SUCCESS);
}

void	ft_end_loop(t_minishell *msh)
{
	ft_putendl_fd("exit", 1);
	msh->loop = 0;
}

int	minishell_loop(t_minishell *msh)
{
	char	*line;

	while (msh->loop)
	{
		signal(SIGINT, &signal_handler);
		line = readline("minishell-$ ");
		if (line != NULL)
		{
			if ((ft_parse(msh, line) != FAILURE && launch_exec(msh) == FAILURE)
				|| g_status == -1)
				return (free(line), clear_history(), FAILURE);
			else
				ft_cmdelst_clear(msh->cmde_lst);
		}
		else
			ft_end_loop(msh);
		if (line && *line)
			add_history (line);
		free(line);
		line = NULL;
	}
	clear_history();
	return (SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	msh;

	g_status = 0;
	(void)ac;
	(void)av;
	signal(SIGQUIT, &signal_handler);
	signal(SIGTSTP, &signal_handler);
	ft_memset(&msh, 0, sizeof(t_minishell));
	msh.loop = 42;
	if (ft_init_envlst(&msh, envp) == FAILURE)
		return (ft_msh_clear(&msh), 1);
	printf("Welcome to my minishell.\n");
	if (minishell_loop(&msh) == FAILURE)
		return (ft_msh_clear(&msh), 1);
	printf("Bye.\n");
	ft_msh_clear(&msh);
	return (g_status);
}
