/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/14 21:42:43 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"
#include <stdio.h>

int	ft_parse_line(t_minishell *msh, char *line)
{
	(void)msh;
	(void)line;
	return (SUCCESS);
}

int	main(void)
{
	char	*line = "echo prout";
	t_minishell msh;

	ft_parse_line(&msh, line);
}
