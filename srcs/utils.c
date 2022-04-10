/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:03:20 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/10 11:00:18 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Retourne 1 si c est un metacharacter cad | < > ou backspace (8 \b) 
horizontal tab (9 \t) line feed (10 \n) vertical tab (11 \v) form feed (12 \f) 
ou carriage return (13 \r) */
int	ft_ismetachar(char c)
{
	if ((c >= 8 && c <= 13) || c == '|' || c == '<' || c == '>' || c == 32)
		return (1);
	return (0);
}

int	ft_isoperator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

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
