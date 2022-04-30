/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:03:20 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 15:27:03 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokens.h"
#include "cmdes.h"

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
	if (prefix)
		ft_putstr_fd(prefix, 2);
	if (suffix)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(suffix, 2);
	}
	ft_putstr_fd("\n", 2);
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
