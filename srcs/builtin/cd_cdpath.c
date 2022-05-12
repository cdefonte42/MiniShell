/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cdpath.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 11:45:12 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/12 09:53:54 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"
#include "export.h"
#include "libft.h"
#include "limits.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include <fcntl.h>

/* Check si le dernier charactere de la string str est egal a c. 
Retourne 1 si oui, 0 sinon */
int	ft_islast_chstr_ch(char *str, char c)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	i = ft_strlen(str) - 1;
	if (str[i] && str[i] == c)
		return (1);
	return (0);
}

/* Test si le dossier 'path' a les droits d'acces en read et execute. 
Retourne 1 si oui, 0 si NON */
int	ft_isadir(char *path)
{
	if (access(path, R_OK | X_OK) != 0)
		return (0);
	return (1);
}

/* Concat each CDPATH pathnames (when CDPATH pathname not NULL) et directory. 
directory should NOT be NULL nor = "\0".
En fonction du cdpath, join direcory avec:
- "./" si *cdpath!=NULL && *cdpath=='\0'
- "/" si cdpath se termine pas par un '/', puis join res avec cdpath
- cdpath directement
Retourne FAILURE si erreur de malloc. */
int	ft_concat_cdpath(char **curpath, char *directory, char *cdpaths)
{
	char	*tmp;

	if (cdpaths[0] == '\0')
		*curpath = ft_strjoin("./", directory);
	else if (!ft_islast_chstr_ch(cdpaths, '/'))
	{
		*curpath = ft_strjoin(cdpaths, "/");
		if (!*curpath)
			return (FAILURE);
		tmp = *curpath;
		*curpath = ft_strjoin(tmp, directory);
		free(tmp);
	}
	else
		*curpath = ft_strjoin(cdpaths, directory);
	if (!*curpath)
		return (FAILURE);
	return (SUCCESS);
}

/* check and test CDPATH ATTENTION CDPATH=:$HOME:$HOME/projects:etc 
1er ele = empty entry = regarde curr directory. si pas ': totu seul des 
le debut alors cherche jamais dans le curr dir */
/*________ STEP 5 ________*/
/* Verifie que pour chaque cdpath value que la concatenation de directory
et ces value est un dir.
Retourne FAILURE si erreur syscall, SUCCESS sinon (si dir pas trouve,
set juste la valeur de curpath a "./" cad curr dir. */
int	ft_try_cdpath(char **curpath, char *directory, t_var *cdpath)
{
	char	**splited_cdpath;
	int		i;

	i = 0;
	splited_cdpath = NULL;
	if (cdpath && cdpath->value)
	{
		splited_cdpath = ft_split(cdpath->value, ':');
		if (!splited_cdpath)
			return (FAILURE);
		while (splited_cdpath && splited_cdpath[i])
		{
			if (!ft_concat_cdpath(curpath, directory, splited_cdpath[i]))
				return (perror("try_cdpath concat failed"), FAILURE);
			if (ft_isadir(*curpath))
				return (ft_free_tabtab(splited_cdpath), SUCCESS);
			i++;
		}
	}
	ft_free_tabtab(splited_cdpath);
	*curpath = ft_strjoin("./", directory);
	if (!*curpath)
		return (FAILURE);
	return (SUCCESS);
}
