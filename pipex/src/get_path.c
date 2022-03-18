/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 15:14:45 by cdefonte          #+#    #+#             */
/*   Updated: 2022/02/23 15:14:01 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Retourne le char **tab de tous les paths indique dans la var env PATH 
Return NULL si malloc error ou si PATH=vide;*/
char	**ft_get_path(char **environ)
{
	char		**path;
	char		*tofind;
	int			i;

	tofind = "PATH=";
	i = 0;
	while (environ && environ[i])
	{
		if (ft_strncmp(environ[i], tofind, 5) == 0)
		{
			path = ft_split(environ[i] + 5, ':');
			if (!path)
				return (NULL);
			else
				return (path);
		}
		i++;
	}
	return (NULL);
}

/* Return concat de path + "/" + cmde; Return NULL si malloc failed */
char	*ft_add_cmde_to_path(char *path, char *cmde)
{
	char	*res;
	int		cmde_len;
	int		path_len;
	int		i;
	int		j;

	cmde_len = ft_strlen(cmde);
	path_len = ft_strlen(path);
	i = -1;
	j = -1;
	res = malloc(sizeof(char) * (cmde_len + path_len + 2));
	if (!res)
		return (NULL);
	while (++i < path_len)
		res[i] = path[i];
	res[i++] = '/';
	while (++j < cmde_len)
		res[i++] = cmde[j];
	res[i] = 0;
	return (res);
}

/* Retoune path+cmde qui mqrche, NULL si pqs trouve ou pas executable */
char	*ft_get_cmde_path(char *cmde, char **paths)
{
	char	*cmde_path;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		cmde_path = ft_add_cmde_to_path(paths[i], cmde);
		if (!cmde_path)
			return (NULL);
		if (access(cmde_path, F_OK) == 0 && access(cmde_path, X_OK) == 0)
			return (cmde_path);
		free(cmde_path);
		cmde_path = NULL;
		i++;
	}
	cmde_path = ft_add_cmde_to_path(".", cmde);
	if (!cmde_path)
		return (NULL);
	if (access(cmde_path, F_OK) == 0 && access(cmde_path, X_OK) == 0)
		return (cmde_path);
	return (free(cmde_path), NULL);
}

/* Retourne un tabtab de cmde1 + arg1 + arg2 + etc */
char	**ft_get_cmde_arg(char *argvi, char **path)
{
	char	**cmde_arg;
	char	*tmp;

	cmde_arg = ft_split(argvi, ' ');
	if (!cmde_arg)
		return (errno = 12, NULL);
	if (cmde_arg[0] == 0)
		return (errno = ESRCH, ft_free_tab(cmde_arg), NULL);
	if (ft_strrchr(cmde_arg[0], '/') == NULL)
	{
		tmp = ft_get_cmde_path(cmde_arg[0], path);
		if (!tmp)
			return (ft_free_tab(cmde_arg), NULL);
		cmde_arg[0] = tmp;
	}
	else if (access(cmde_arg[0], F_OK) != 0 || access(cmde_arg[0], X_OK) != 0)
		return (ft_free_tab(cmde_arg), NULL);
	return (cmde_arg);
}

void	ft_expansion(char ***cmde_args, char ***paths, char **environ, \
t_cmde *cmde)
{
	*paths = ft_get_path(environ);
	if (!paths)
		exit(ft_error_handler("ft_expansion", &cmde, command));
	*cmde_args = ft_get_cmde_arg(cmde->argvi, *paths);
	if (!*cmde_args)
	{
		if (errno == ENOENT)
			errno = ESRCH;
		ft_free_tab(*paths);
		exit(ft_error_handler(cmde->argvi, &cmde, command));
	}
}
