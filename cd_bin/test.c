/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:11:46 by cdefonte          #+#    #+#             */
/*   Updated: 2022/03/22 11:53:46 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "limits.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdio.h"
#include <fcntl.h>

int	ft_isadir(char *path)
{
	if (access(path, F_OK) == 0 && access(path, R_OK) == 0)
		return (1);
	else
		return (perror("isadir nope"), 0);
}

/* Check si le dernier charactere de la string str est egal a c. 
Retourne son index, sinon -1. Ne MARCHE PAS avec '\0' */
int	ft_islast_chstr_ch(char *str, char c)
{
	int		i;

	i = 0;
	if (!str)
		return (-1);
	i = ft_strlen(str) - 1;
	while (i >= 0)
	{
		if (str[i] == c)
			return (i);
		i--;
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
		printf("step 7 1\n");
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
		printf("step 7 2\n");
		tmp = curpath;
		curpath = ft_strjoin(pwdstr, tmp);
		free(tmp);
		if (!curpath)
			return (NULL);
	}
	printf("step 7 return curpath = %s\n", curpath);
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
			printf("4332532\n");
			curpath = ft_strjoin("./", directory);
			if (!curpath)
				return (NULL);
		}
		else if (ft_islast_chstr_ch(splited_cdpathname[i], '/') != -1) // si '/' est PAS a la fin
		{
			printf("24325324\n");
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
			printf("0942013\n");
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
char	*ft_try_cdpath(char *directory)
{
	char	*cdpathname;
	char	*curpath;

	cdpathname = getenv("CDPATH");
	if (cdpathname != NULL) //1ere partie
	{
		printf("step 5 1\n");
		curpath = ft_test_concat_dir(cdpathname, directory);
		if (!curpath) // STEP 6 
			return (NULL);
		if (ft_isadir(curpath))
			return (curpath);
	}
	else // cdpathname is NULL, 2eme partie
	{
		printf("step 5 2\n");
		curpath = ft_strjoin("./", directory);
		if (!curpath)
			return (NULL);
		if (ft_isadir(curpath))
			return (curpath);
	}
	return (NULL);
}

/*________ STEPS 1 and 2 __________*/
char	*ft_cd_home(void)
{
	char	*home;
	char	*curpath;

	home = getenv("HOME"); //getenv() donne la str apres le '=' de "HOME"
	if (home == NULL) // => HOME undefined
		return (ft_putstr_fd("HOME not set",2), NULL); //A FAIRE par error handler (return statusest de 1)
	else if (*home == 0) // HOME empty
		return (NULL);
	curpath = ft_strdup(home);
	if (!curpath)
		return (NULL);
	return (curpath);
}

int	ft_cd(char *directory)
{
	char	*curpath;

	curpath = NULL;
	if (directory == NULL) //steps 1 and 2
	{
		printf("directory=NULL\n");
		curpath = ft_cd_home();
		if (!curpath)
			return (-1);
	}
	else if (*directory == 0)
	{
		printf("directory='0'\n");
		return (0);
	}
	else if (*directory == '/' || *directory == '.' || (*(directory + 1) 
		&& *directory == '.' && *(directory + 1) == '.'))
	{
		curpath = ft_strdup(directory);
		if (!curpath)
			return (-1);
	}
	else
	{
		curpath = ft_try_cdpath(directory);
		if (!curpath)
			return (-1);
	}
	/*______ STEP 9 ______*/
	printf("curpath tested = %s\n", curpath);
	if (curpath && ft_strlen(curpath) + 1 > PATH_MAX) // FAIRE STEP 9
		return (perror("supp PATH_MAX"), -1);
	if (chdir(curpath) != 0)
		return (perror("(chdir) cd"), -1);
	free(curpath);
	return (0);
}

int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int		debug = 1;

	if (!debug)
	{
		printf("AVANT cd pwd = %s\n", getcwd(NULL, 0));
		if (ft_cd(*(argv + 1)) != 0) //ERROR HANDLER
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
