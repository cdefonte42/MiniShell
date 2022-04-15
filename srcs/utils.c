/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:03:20 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/15 12:01:57 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokens.h"
#include "cmdes.h"

/* In the shell command language, a word consisting solely of underscores, 
digits, and alphabetics from the portable character set. 
The first character of a name is not a digit. */
int	ft_isname(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] >= '0' && str[0] <= '9')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_tabtab(char **tab)
{
	int		i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;	
	}
	free(tab);
}

int	ft_isblank(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

/* Compare c avec chaque caractere de set. Retourne 1 si c est parmis set. 0 si
c ne fait pas partis du set de caracteres. */
int	ft_isset(char c, char *set)
{
	int		i;

	i = 0;
	while (set && set[i])
	{
		if (c == set[i])
			return (1);
		i++;	
	}
	return (0);
}

/* Retourne 1 si 'name' est le nom d'un buildin, 0 sinon */
int	ft_isbin(char *name)
{
	char	*bin[7];
	int		i;

	bin[0] = "echo";
	bin[1] = "cd";
	bin[2] = "pwd";
	bin[3] = "export";
	bin[4] = "unset";
	bin[5] = "env";
	bin[6] = "exit";
	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(name, bin[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

