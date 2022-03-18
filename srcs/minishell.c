/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:26:30 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/18 11:42:12 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
# define C_RESET "\033[0m"
# define C_BLUE "\033[0;34m"
# define VERSION "0.1"

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

void	minishell_loop()
{
	char *line;

	while (1)
	{
		line = readline(C_BLUE"minishell-"VERSION C_RESET"$ ");
		if (line == NULL)
			write(1, "exit\n", 5);
		if ((ft_strncmp(line, "exit", 4) == 0))
			break ;
		printf("%s\n", line);
		free(line);
	}
	free(line);
	rl_clear_history();	
}

int	main(int ac, char *av[], char *env[])
{
	(void) ac;
	(void) av;
	(void) env;

	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, &signal_handler);
	signal(SIGTSTP, &signal_handler);
	printf("%p\n", __builtin_frame_address(0));
	minishell_loop();
	printf("Welcome to my shell\n");
	return (0);
}
