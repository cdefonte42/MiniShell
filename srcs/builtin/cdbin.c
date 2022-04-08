/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cdbin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:11:46 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/08 15:27:53 by cdefonte         ###   ########.fr       */
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

/* Print le message d'erreur specifique a 'cd'. Peut etre change en return int
pour return exit status = 128 + errno */
static void	ft_error_handler(char *directory)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("cd: ", 2);
	if (directory && *directory)
	{
		ft_putstr_fd(directory, 2);
		ft_putstr_fd(": ", 2);
	}
	perror(NULL);
}

/* Fait cd sans aucun. Retourne 0 si tout s'est bien passe et set la val de
curpath a envoyer a chdir apres appel de ft_cd_home. Attention peut renvoyer 0
et curpath = NULL, ca veut pas dire pb.
Retourne -1 en cas d'erreur de malloc. */
/*________ STEPS 1 and 2 __________*/
int	ft_cd_home(char **curpath)
{
	char	*home;

	home = getenv("HOME");
	if (home == NULL)
		return (ft_putstr_fd("HOME not set", 2), FAILURE);
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
		return ;
	free(var->value);
	var->value = strtoput;
}

/* Launch la bonne fonction en fonction de la valeur de l'arg 'directory' */
int	ft_parse_dir(char **curpath, char *directory)
{
	if (directory == NULL)
	{
		if (ft_cd_home(curpath) == FAILURE)
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
		if (ft_try_cdpath(curpath, directory, getenv("CDPATH")) == FAILURE)
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
	oldpwd = getcwd(NULL, 0);
	if (oldpwd == NULL)
		return (perror("getcwp oldpwd debut ft_cd"), FAILURE);
	if (ft_parse_dir(&curpath, directory[1]) == FAILURE)
		return (free(oldpwd), FAILURE);
	if (curpath && ft_strlen(curpath) + 1 > PATH_MAX)
		return (free(oldpwd), perror("supp PATH_MAX"), FAILURE);
	if (curpath && chdir(curpath) != 0)
		return (free(oldpwd), ft_error_handler(directory[1]), FAILURE);
	free(curpath);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (free(oldpwd), perror("getcwp pwd ft_cd"), FAILURE);
	ft_maj_varenvstr(*var_lst, "PWD", pwd);
	ft_maj_varenvstr(*var_lst, "OLDPWD", oldpwd);
	return (SUCCESS);
}
//
//int	minishell_get_env(t_var **vars, char **envp)
//{
//	int		i;
//	char	**splited;
//
//	i = 0;
//	if (!envp)
//		return (FAILURE);
//	i = 0;
//	while (envp && envp[i])
//	{
//		splited = ft_split(envp[i], '=');
//		if (!ft_new_var(vars, splited[0], splited[1], 0))
//			return (free(splited), FAILURE);
//		free(splited);
//		i++;
//	}
//	return (SUCCESS);
//}
//
//int	main(int argc, char **argv, char **envp)
//{
//	char	*directory;
//	t_var	*var_lst;
//	int		debug = 0;
//
//	minishell_get_env(&var_lst, envp);
//	if (argc == 1)
//		directory = NULL;
//	else
//		directory = *(argv + 1);
//	if (!debug)
//	{
//		printf("AVANT cd pwd = %s\n", getcwd(NULL, 0));
//		if (ft_cd(directory, var_lst) != 0) //ERROR HANDLER
//		{
//			printf("APRES cd pwd = %s\n", getcwd(NULL, 0));
//			return (1);
//		}
//		printf("APRES cd pwd = %s\n", getcwd(NULL, 0));
//	}
//	else
//	{
//		char	*str;
//
//		str = getcwd(NULL, 0);
//		if (!str)
//			return (perror("NISNDI"), 1);
//		printf("AVANT cd pwd = %s\n", str);
//		if (chdir(*(argv + 1)) != 0)
//			return (perror("(chdir) cd"), -1);
////		str = getcwd(NULL, 0);
////		if (!str)
////			return (perror("NISNDI"), 1);
////		printf("APRES cd pwd = %s\n", str);
//		free(str);
//	}
//	return (0);
//}
