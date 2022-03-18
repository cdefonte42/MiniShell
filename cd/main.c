/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 14:11:46 by cdefonte          #+#    #+#             */
/*   Updated: 2022/03/18 18:34:00 by cdefonte         ###   ########.fr       */
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

int	ft_cd_home(char ***env)
{
	char	*home;

	home = ft_varenv_str(env, "HOME");
	if (home == NULL) // => HOME unset
		return (ft_putstr_fd("HOME not set",2), -1); //A FAIRE par error handler (return statusest de 1)
	else if (*home == 0)
		return (0);
	if (chdir(home) != 0)
		return (perror("chdir failed"), -1);
	return (0);
}

int	ft_cd(char *path, char ***env)
{
	if (path == NULL)
	{
		if (ft_cd_home(env) != 0)
			return (-1);
	}
	else if (*path == 0)
		return (0);
	//chdir PATH
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char ***env;
	(void)argc;

	env = minishell_get_env(envp);
	if (ft_cd(*(argv + 1), env) != 0) //ERROR HANDLER
		return (1);
	printf("LA %d\n", PATH_MAX);
	printf("%s\n", getcwd(NULL, 0));
	return (0);
}
