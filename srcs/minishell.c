/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:26:30 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/18 09:49:48 by mbraets          ###   ########.fr       */
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
	if (signalid == 2)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signalid == 3 || signalid == 20)
	{
		write(1, "\b\b", 2);
		write(1, "  ", 2);
		write(1, "\b\b", 2);
	}
	// printf("%d", signalid);
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
	char *line;
	while (1)
	{
		line = readline(C_BLUE"minishell-"VERSION C_RESET"$ ");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (ft_strncmp(line, "exit", 4) == 0)
			break ;
		printf("%s\n", line);
		free(line); 
	}
	free(line);
	rl_clear_history();
	printf("Welcome to my shell\n");
	return (0);
}
