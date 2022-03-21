/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:11:46 by cdefonte          #+#    #+#             */
/*   Updated: 2022/03/21 16:46:02 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "limits.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"

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
d'env 'var_name' passee en param. ATTENTION il faut donner le '=' ds var_name.
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
		{
			printf("dans varenv %s\n", env[i][1]);
			return (env[i][1]);
		}
		i++;
	}
	return (NULL);
}

/*________ STEPS 1 and 2 __________*/
int	ft_cd_home(void)
{
	char	*home;

	home = getenv("HOME"); //getenv() donne la str apres le '=' de "HOME"
	if (home == NULL) // => HOME undefined
		return (ft_putstr_fd("HOME not set",2), -1); //A FAIRE par error handler (return statusest de 1)
	else if (*home == 0) // HOME empty
		return (0);
	if (chdir(home) != 0)
		return (perror("chdir failed"), -1);
	return (0);
}

/* Check si le dernier charactere de la string str est egal a c. 
Retourne son index, sinon -1. Ne MARCHE PAS avec '\0' */
int	ft_islast_chstr_ch(char *str, char c)
{
	int		i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
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
	if (ft_islast_chstr_ch(pwdstr, '/') == -1) // si pwdstr se termine PAS par un slash
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
char	*ft_test_concat_dir(char *cdpathname, char toadd, char *directory)
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
		else if (ft_islast_chstr_ch(splited_cdpathname[i], '/') != -1) // si '/' est PAS a la fin
		{
			curpath = ft_split(splited_cdpathname[i], "/");
			if (!curpath)
				return (NULL);
			tmp = curpath;
			curpath = ft_split(tmp, directory);
			free(tmp);
			if (!curpath)
				return (NULL);
		}
		else // si pathname != NULL et a le '/' a la fin
		{
			curpath = ft_split(splited_cdpathname[i], directory);
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
char	*ft_try_cdpath(char *directory)
{
	char	*cdpathname;
	char	*curpath;

	cdpathname = getenv("CDPATH");
	if (cdpathname != NULL) //1ere partie
	{
		curpath = ft_test_concat_dir(cdpathname, '/', directory);
		if (!curpath) // STEP 6 
			return (NULL);
	}
	else // cdpathname is NULL, 2eme partie
	{
		curpath = ft_strjoin("./", directory);
		if (!curpath)
			return (NULL);
	}
	if (ft_isadir(curpath))
		return (curpath);
	else
		return (NULL);
}

int	ft_cd(char *directory)
{
	char	*curpath;

	if (directory == NULL) //steps 1 and 2
	{
		if (ft_cd_home() != 0)
			return (-1);
	}
	else if (*directory == 0)
		return (0);
	/*____ STEP 5 ____*/
	curpath = ft_try_cdpath(directory); //Si directory commence pas part '/' '.' ni '..' 
	if (!curpath)
		curpath = directory;
	if (curpath[0] && curpath[0] != '/') //si curpath commence PAS par slash
	{
		curpath = ft_try_cdpath
	}
	//chdir PATH
	return (0);
}

int	main(int argc, char **argv)
{
	(void)argc;

	printf("AVANT %s\n", getcwd(NULL, 0));
	if (ft_cd(*(argv + 1)) != 0) //ERROR HANDLER
		return (1);
	char	*curr_path;
	
	curr_path = getcwd(NULL, 0);
	printf("APRES %s\n", curr_path);
	free(curr_path); // Leaks wtf
	return (0);
}
