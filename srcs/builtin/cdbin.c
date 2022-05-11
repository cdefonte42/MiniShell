/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cdbin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:11:46 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/11 15:26:06 by cdefonte         ###   ########.fr       */
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

/* Fait cd sans aucun. Retourne 0 si tout s'est bien passe et set la val de
curpath a envoyer a chdir apres appel de ft_cd_home. Attention peut renvoyer 0
et curpath = NULL, ca veut pas dire pb.
Retourne -1 en cas d'erreur de malloc. */
/*________ STEPS 1 and 2 __________*/
int	ft_cd_home(t_var *var_lst, char **curpath)
{
	char	*home;

	home = var_getvaluefromkey(var_lst, "HOME");
	if (home == NULL)
		return (ft_error("cd", "HOME not set"), FAILURE);
	else if (*home == 0)
		return (SUCCESS);
	*curpath = ft_strdup(home);
	if (!*curpath)
		return (FAILURE);
	return (SUCCESS);
}

/* Cherche l'element de la liste 'var_lst' qui a la 'key' egale a 'varname',
et remplace sa 'value' par 'strtoput'. Free l'ancienne 'value'. 'strtoput' doit
etre une chaine allouee. */
void	ft_maj_varenvstr(t_var *var_lst, char *varname, char *strtoput)
{
	t_var	*var;

	if (!strtoput || !varname || !var_lst)
		return ;
	var = var_getfromkey(var_lst, varname);
	if (!var)
	{
		free(strtoput);
		strtoput = NULL;
		return ;
	}
	free(var->value);
	var->value = strtoput;
}

/* Launch la bonne fonction en fonction de la valeur de l'arg 'directory' */
int	ft_parse_dir(char **curpath, char *directory, t_var *lst)
{
	t_var	*cdpath;

	if (directory == NULL)
	{
		if (ft_cd_home(lst, curpath) == FAILURE)
			return (FAILURE);
	}
	else if (*directory == '/' || *directory == '.' )
	{
		*curpath = ft_strdup(directory);
		if (!*curpath)
			return (FAILURE);
	}
	else
	{
		cdpath = var_getfromkey(lst, "CDPATH");
		if (ft_try_cdpath(curpath, directory, cdpath) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	ft_cd(t_var **var_lst, char **directory)
{
	char	*curpath;
	char	*oldpwd;
	char	*pwd;

	curpath = NULL;
	errno = 0;
	oldpwd = getcwd(NULL, 0);
	if (oldpwd == NULL && errno == 12)
		return (perror("minishell: cd: getcwd: malloc error"), -1);
	if (ft_parse_dir(&curpath, directory[1], *var_lst) == FAILURE)
		return (free(oldpwd), 1);
	if (curpath && ft_strlen(curpath) + 1 > PATH_MAX)
		return (free(oldpwd), free(curpath), perror("supp PATH_MAX"), 1);
	if (curpath && chdir(curpath) != 0)
		return (free(oldpwd), free(curpath), ft_perror("cd", directory[1]), 1);
	free(curpath);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL && errno == 12)
		return (free(oldpwd), perror("minishell: cd: getcwd:malloc error"), -1);
	ft_maj_varenvstr(*var_lst, "PWD", pwd);
	ft_maj_varenvstr(*var_lst, "OLDPWD", oldpwd);
	return (0);
}
