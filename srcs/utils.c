/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:03:20 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/14 21:23:13 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_cisname(char c)
{
	if (!c)
		return (0);
	if (c >= '0' && c <= '9')
		return (0);
	if (!ft_isalnum(c) && c != '_')
		return (0);
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
