/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:26:30 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/16 11:50:45 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char *av[], char *env[])
{
	(void) ac;
	(void) av;
	while (*env)
		printf("%s\n", *(env++));

	char *line;	
	while ((line = readline("minishell$ ")))
		printf("%s\n", line);
	printf("Welcome to my shell\n");
	return (0);
}
