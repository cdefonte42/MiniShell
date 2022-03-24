/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cdbin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:11:46 by cdefonte          #+#    #+#             */
/*   Updated: 2022/03/24 10:03:54 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "limits.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include <fcntl.h>

/* Print le message d'erreur specifique a 'cd'. Peut etre change en return int
pour return exit status = 128 + errno */
void	ft_error_handler(char *directory)
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

/* Return la str (non allouee) apres le signe '=' de la variable 
d'env 'var_name' passee en param. 
Si apres le '=', le char est '\0', on return une str = "" */
// TEUBEE LA FONCION EXISTE ET EST AUTORISEE CEST GETENV() 
char	*ft_varenv_str(char ***env, char *var_name)
{
	int		i;

	i = 0;
	if (!env || !*env || !var_name)
		return (NULL);
	while (env[i])
	{
		if (ft_strcmp(env[i][0], var_name) == 0)
			return (env[i][1]);
		i++;
	}
	return (NULL);
}

/* Test si le dossier 'path' a les droits d'acces en read et execute. 
Retourne 1 si oui, 0 si NON */
int	ft_isadir(char *path)
{
	if (access(path, R_OK | X_OK) != 0)
		return (0);
	return (1);
}

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

///* USELESS FCT? Devrait se faire apres avoir try tous les chemins de CDPATH, 
//mais en vrai chdir en a pas besoin */
///*______________ STEP 7 _________*/
//char	*ft_try_pwd(char *curpath)
//{
//	char	*pwdstr;
//	char	*tmp;
//	char	*tmpbis;
//
//	pwdstr = getenv("PWD");
//	if (!pwdstr)
//		return (free(curpath), NULL);
//	if (!ft_islast_chstr_ch(pwdstr, '/')) // si pwdstr se END PAS par un slash
//	{
//		tmp = ft_strjoin(pwdstr, "/");
//		if (!tmp)
//			return (free(curpath), NULL);
//		tmpbis = curpath;
//		curpath = ft_strjoin(tmp, tmpbis);
//		free(tmp);
//		free(tmpbis);
//		if (!curpath)
//			return (NULL);
//	}
//	else // pwdstr se termine par slash
//	{
//		tmp = curpath;
//		curpath = ft_strjoin(pwdstr, tmp);
//		free(tmp);
//		if (!curpath)
//			return (NULL);
//	}
//	return (curpath);
//}

/* Concat each CDPATH pathnames (when CDPATH pathname not NULL) et directory. 
directory should NOT be NULL nor = "\0".*/
/*______________ STEP 5 ____________*/
int	ft_test_concat_dir(char ** curpath, char *cdpathname, char *directory)
{
	char	**splited_cdpathname;
	char	*tmp;
	int		i;

	i = 0;
	splited_cdpathname = ft_split(cdpathname, ':');
	if (!splited_cdpathname)
		return (-1);
	while (splited_cdpathname[i])
	{
		if (splited_cdpathname[i][0] == 0) // si splited_cdpathname[i] = "\0", doit mettre "./"
		{
			*curpath = ft_strjoin("./", directory);
			if (!*curpath)
				return (-1);
		}
		else if (!ft_islast_chstr_ch(splited_cdpathname[i], '/')) // si '/' est PAS a la fin
		{
			*curpath = ft_strjoin(splited_cdpathname[i], "/");
			if (!*curpath)
				return (-1);
			tmp = *curpath;
			*curpath = ft_strjoin(tmp, directory);
			free(tmp);
			if (!*curpath)
				return (-1);
		}
		else // si pathname != NULL et a le '/' a la fin
		{
			*curpath = ft_strjoin(splited_cdpathname[i], directory);
			if (!*curpath)
				return (-1);
		}
		if (ft_isadir(*curpath)) //si curpath exists
			return (0);
		i++;
	}
	return (1); //FILE NOT FOUND
}

/* check and test CDPATH ATTENTION CDPATH=:$HOME:$HOME/projects:etc 
1er ele = empty entry = regarde curr directory. si pas ': totu seul des 
le debut alors cherche jamais dans le curr dir */
/*________ STEP 5 ________*/
int	ft_try_cdpath(char **curpath, char *directory, char ***env)
{
	char	*cdpathstr;

	cdpathstr = ft_varenv_str(env, "CDPATH");
	if (cdpathstr != NULL) //1ere partie
	{
		if (ft_test_concat_dir(curpath, cdpathstr, directory) == -1)
			return (-1);
		return (0);
	}
	else // cdpathstr is NULL, 2eme partie
	{
		*curpath = ft_strjoin("./", directory);
		if (!*curpath)
			return (-1);
		return (0);
	}
}

/* Fait cd sans aucun. Retourne 0 si tout s'est bien passe et set la val de
curpath a envoyer a chdir apres appel de ft_cd_home. Attention peut renvoyer 0
et curpath = NULL, ca veut pas dire pb.
Retourne -1 en cas d'erreur de malloc. */
/*________ STEPS 1 and 2 __________*/
int ft_cd_home(char **curpath)
{
	char	*home;

	home = getenv("HOME"); //getenv() donne la str apres le '=' de "HOME"
	if (home == NULL) // => HOME undefined
		return (ft_putstr_fd("HOME not set",2), -1); //A FAIRE par error handler (return statusest de 1)
	else if (*home == 0) // HOME empty
		return (0);
	*curpath = ft_strdup(home);
	if (!*curpath)
		return (-1);
	return (0);
}

int	ft_cd(char *directory, char ***env)
{
	char	*curpath;

	curpath = NULL;
	if (directory == NULL)
	{
		if (ft_cd_home(&curpath) == -1)
			return (-1);
	}
	else if (*directory == '/' || *directory == '.' )
	{
		curpath = ft_strdup(directory);
		if (!curpath)
			return (-1);
		//else if (curpath == NULL) => join with PWD
	}
	else
	{
		if (ft_try_cdpath(&curpath, directory, env) == -1)
			return (-1);
	}
	if (curpath && ft_strlen(curpath) + 1 > PATH_MAX) // STEP 9 mais en vrai chdir doit s'en occuper tout seul non ?
		return (perror("supp PATH_MAX"), -1);
	if (curpath && chdir(curpath) != 0)
		return (ft_error_handler(directory), -1);
	free(curpath);
	return (0);
}

/*
char***    minishell_get_env(char **envp)
{
    int    i;
    char ***env;
    i = 0;
    while (envp && envp[i])
        i++;
    env = malloc(sizeof(char **) * (i + 1));
    env[i] = NULL;
    while (i--)
        env[i] = ft_split(envp[i], '=');
    return (env);
}

int	main(int argc, char **argv, char **envp)
{
	char	*directory;
	char	***env;
	int		debug = 0;

	env = minishell_get_env(envp);
	if (argc == 1)
		directory = NULL;
	else
		directory = *(argv + 1);
	if (!debug)
	{
		printf("AVANT cd pwd = %s\n", getcwd(NULL, 0));
		if (ft_cd(directory, env) != 0) //ERROR HANDLER
		{
			printf("APRES cd pwd = %s\n", getcwd(NULL, 0));
			return (1);
		}
		printf("APRES cd pwd = %s\n", getcwd(NULL, 0));
	}
	else
	{
		char	*str;

		str = getcwd(NULL, 0);
		if (!str)
			return (perror("NISNDI"), 1);
		printf("AVANT cd pwd = %s\n", str);
		if (chdir(*(argv + 1)) != 0)
			return (perror("(chdir) cd"), -1);
//		str = getcwd(NULL, 0);
//		if (!str)
//			return (perror("NISNDI"), 1);
//		printf("APRES cd pwd = %s\n", str);
		free(str);
	}
	return (0);
}

*/
