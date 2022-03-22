/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:11:46 by cdefonte          #+#    #+#             */
/*   Updated: 2022/03/22 15:51:22 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "limits.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include <fcntl.h>

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

/*______________ STEP 7 _________*/
char	*ft_try_pwd(char *curpath)
{
	char	*pwdstr;
	char	*tmp;
	char	*tmpbis;

	pwdstr = getenv("PWD");
	if (!pwdstr)
		return (free(curpath), NULL);
	if (!ft_islast_chstr_ch(pwdstr, '/')) // si pwdstr se termine PAS par un slash
	{
		tmp = ft_strjoin(pwdstr, "/");
		if (!tmp)
			return (free(curpath), NULL);
		tmpbis = curpath;
		curpath = ft_strjoin(tmp, tmpbis);
		free(tmp);
		free(tmpbis);
		if (!curpath)
			return (NULL);
	}
	else // pwdstr se termine par slash
	{
		tmp = curpath;
		curpath = ft_strjoin(pwdstr, tmp);
		free(tmp);
		if (!curpath)
			return (NULL);
	}
	return (curpath);
}

/* Concat each CDPATH pathnames (when CDPATH pathname not NULL) et directory. 
directory should NOT be NULL nor = "\0".*/
/*______________ STEP 5 ____________*/
char	*ft_test_concat_dir(char *cdpathname, char *directory)
{
	char	**splited_cdpathname;
	char	*curpath;
	char	*tmp;
	int		i;

	i = 0;
	splited_cdpathname = ft_split(cdpathname, ':');
	if (!splited_cdpathname)
		return (NULL);
	while (splited_cdpathname[i])
	{
		if (splited_cdpathname[i][0] == 0) // si splited_cdpathname[i] = "\0", doit mettre "./"
		{
			curpath = ft_strjoin("./", directory);
			if (!curpath)
				return (NULL);
		}
		else if (!ft_islast_chstr_ch(splited_cdpathname[i], '/')) // si '/' est PAS a la fin
		{
			curpath = ft_strjoin(splited_cdpathname[i], "/");
			if (!curpath)
				return (NULL);
			tmp = curpath;
			curpath = ft_strjoin(tmp, directory);
			free(tmp);
			if (!curpath)
				return (NULL);
		}
		else // si pathname != NULL et a le '/' a la fin
		{
			curpath = ft_strjoin(splited_cdpathname[i], directory);
			if (!curpath)
				return (NULL);
		}
		if (ft_isadir(curpath)) //si curpath exists
			return (curpath);
		i++;
	}
	return (NULL);
}

// check and test CDPATH ATTENTION CDPATH=:$HOME:$HOME/projects:etc 1er ele = empty entry = regarde curr directory. si pas ': totu seul des le debut alors cherche jamais dans le curr dir
/*________ STEP 5 ________*/
char	*ft_try_cdpath(char *directory, char ***env)
{
	char	*cdpathname;
	char	*curpath;

	cdpathname = ft_varenv_str(env, "CDPATH");
	if (cdpathname != NULL) //1ere partie
	{
		curpath = ft_test_concat_dir(cdpathname, directory);
		if (!curpath) // STEP 6 
			return (NULL);
		return (curpath);
	}
	else // cdpathname is NULL, 2eme partie
	{
		curpath = ft_strjoin("./", directory);
		if (!curpath)
			return (NULL);
		return (curpath);
	}
}

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
	if (directory == NULL) //steps 1 and 2
	{
		if (ft_cd_home(&curpath) == -1)
			return (-1);
		else if (!curpath) // si pas d'erreur de malloc mais que *home==rien
			return (0);
	}
	else if (*directory == 0)
		return (0);
	else if (*directory == '/' || *directory == '.' || (*(directory + 1) 
		&& *directory == '.' && *(directory + 1) == '.'))
	{
		curpath = ft_strdup(directory);
		if (!curpath)
			return (-1);
	}
	else
	{
		curpath = ft_try_cdpath(directory, env);
		if (!curpath)
			return (-1);
	}
	if (curpath && ft_strlen(curpath) + 1 > PATH_MAX) // FAIRE STEP 9
		return (perror("supp PATH_MAX"), -1);
	if (chdir(curpath) != 0)
		return (perror("(chdir) cd"), ft_putstr_fd(curpath, 2), -1);
	free(curpath);
	return (0);
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
		str = getcwd(NULL, 0);
		if (!str)
			return (perror("NISNDI"), 1);
		printf("APRES cd pwd = %s\n", str);
		free(str);
	}
	return (0);
}
