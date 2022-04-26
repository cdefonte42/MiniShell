/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:03:20 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/26 10:37:20 by cdefonte         ###   ########.fr       */
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

int	ft_cisname(char c)
{
	if (!c)
		return (0);
	if (!ft_isalnum(c) && c != '_')
		return (0);
	return (1);
}

int	ft_fcisname(char c)
{
	if (!c)
		return (0);
	if (ft_isdigit(c))
		return (0);
	else if (!ft_isalpha(c) && c != '_')
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
		tab[i] = NULL;
		i++;
	}
	if (tab)
	{
		free(tab);
		tab = NULL;
	}
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

/* Print le message d'erreur. Peut etre change en return int
pour return exit status = 128 + errno */
void	ft_perror(char *cmd, char *suffix)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	perror(suffix);
}

/* Print le message d'erreur. Peut etre change en return int
pour return exit status = 128 + errno */
void	ft_error(char *prefix, char *suffix)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(prefix, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(suffix, 2);
}

