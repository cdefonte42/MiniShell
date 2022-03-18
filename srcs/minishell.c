/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:26:30 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/18 17:40:08 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct test {

} t_test;

// SIGINT == 2

void	signal_handler(int signalid)
{
	if (signalid == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signalid == SIGQUIT || signalid == SIGTSTP)
		write(1, "\b\b  \b\b", 6);
	// printf("%d", signalid);
}

void	fexit(t_minishell *msh)
{
	int	returnvalue;
	// DEBUG && printf("wfjug\n");
	returnvalue = msh->status;
	exit(returnvalue);
}

void	builtin_exec(t_minishell *msh, char *line)
{
	char	**line_args;

	line_args = ft_split(line, ' ');
	if (line_args == NULL)
		return ;
	if ((ft_strcmp(line_args[0], "exit") == 0))
	{
		if (line_args[1] != NULL)
			msh->status = ft_atoi(line_args[1]);
		fexit(msh);
	}
	// if ((ft_strcmp(line_args[0], "cd") == 0))
	// {
	// 	ft_cd(msh);
	// }
}

void	minishell_get_env(t_minishell *msh, char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	msh->env = malloc(sizeof(char **) * i + 1);
	msh->env[i] = NULL;
	while (i--)
		msh->env[i] = ft_split(envp[i], '=');
}


void	minishell_loop(t_minishell *msh)
{
	char *line;

	while (1)
	{
		line = readline(C_BLUE"minishell-"VERSION C_RESET"$ ");
		if (line == NULL)
		{
			ft_putendl_fd("exit", 1);
			line = "exit";
			msh->status = 130;
		}
		builtin_exec(msh, line);
		if (line && *line)
			add_history (line);
		free(line);
	}
	free(line);
	rl_clear_history();
}

int	main(int ac, char *av[], char *envp[])
{
	(void)		ac;
	(void)		av;
	t_minishell	msh;
	minishell_get_env(&msh, envp);
	msh = (t_minishell){.status = 0};
	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, &signal_handler);
	signal(SIGTSTP, &signal_handler);
	printf("Welcome to my minishell.\n");
	minishell_loop(&msh);
	printf("Bye.\n");
	return (msh.status);
}
