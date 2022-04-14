/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:26:30 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/14 21:43:05 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

// SIGINT == 2

int	g_status;

void	signal_handler(int signalid)
{
	if (signalid == SIGINT)
	{
		write(1, "\n", 1);
	//	rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 130;
	}
	if (signalid == SIGQUIT || signalid == SIGTSTP)
		write(1, "\b\b  \b\b", 6);
}

int	ft_init_envlst(t_minishell *msh, char **envp)
{
	int			i;
	char		**splited;
	t_var_type	type;

	i = 0;
	if (!envp)
		return (FAILURE);
	while (envp && envp[i])
	{
		type = envvar;
		splited = ft_split(envp[i], '=');
		if (!splited)
			return (perror("ft_init_envlst failed split"), FAILURE);
		if (ft_strcmp(splited[0], "_") == 0)
			type = shellvar;
		if (ft_new_var(&(msh->vars), splited[0], splited[1], type) == FAILURE)
			return (ft_free_tabtab(splited), FAILURE);
		free(splited);
		i++;
	}
	return (SUCCESS);
}

int	minishell_loop(t_minishell *msh)
{
	char	*line;

	while (msh->loop)
	{
		line = readline(C_BLUE"minishell-"VERSION C_RESET"$ ");
		if (line == NULL)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		else
		{
			ft_parse_line(msh, line);
		}
		free(line);
	}
	clear_history();
	return (SUCCESS);
}

//int	main(int ac, char *av[], char *envp[])
//{
//	(void)		ac;
//	(void)		av;
//	g_status = 0;
//	t_minishell	msh;
//	msh = (t_minishell){.loop = 42};
//	if (ft_init_envlst(&msh, envp) == FAILURE)
//		return (1);
//	signal(SIGINT, &signal_handler);
//	signal(SIGQUIT, &signal_handler);
//	signal(SIGTSTP, &signal_handler);
//	printf("Welcome to my minishell.\n");
//	minishell_loop(&msh);
//	printf("Bye.\n");
//	return (g_status);
//}
