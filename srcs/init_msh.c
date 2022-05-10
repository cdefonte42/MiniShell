/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 17:45:23 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/10 12:42:23 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

static int	new_var(t_minishell *msh, char *splited[2])
{
	t_var_type	type;

	type = envvar;
	if (ft_strcmp(splited[0], "_") == 0)
		type = shellvar;
	if (ft_new_var(&(msh->vars), splited[0], splited[1], type) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	msh_init_envi(t_minishell *msh)
{
	char	*keynval[2];

	keynval[0] = ft_strdup("OLDPWD");
	if (!keynval[0])
		return (FAILURE);
	keynval[1] = NULL;
	if (new_var(msh, keynval) == FAILURE)
		return (free(keynval[0]), free(keynval[1]), FAILURE);
	keynval[0] = ft_strdup("PWD");
	if (!keynval[0])
		return (FAILURE);
	keynval[1] = getcwd(NULL, 0);
	if (!keynval[1] && errno == 12)
		return (free(keynval[0]), FAILURE);
	if (new_var(msh, keynval) == FAILURE)
		return (free(keynval[0]), free(keynval[1]), FAILURE);
	keynval[0] = ft_strdup("SHLVL");
	if (!keynval[0])
		return (FAILURE);
	keynval[1] = ft_strdup("1");
	if (!keynval[1])
		return (free(keynval[0]), FAILURE);
	if (new_var(msh, keynval) == FAILURE)
		return (free(keynval[0]), free(keynval[1]), FAILURE);
	return (SUCCESS);
}

int	ft_init_envlst(t_minishell *msh, char **envp)
{
	int			i;
	char		*splited[2];
	int			j;

	i = 0;
	ft_memset(splited, 0, sizeof(char *) * 2);
	if (!envp || (!*envp && msh_init_envi(msh) == FAILURE))
		return (FAILURE);
	while (envp && *envp && envp[i])
	{
		j = 0;
		while (envp[i] && envp[i][j] != '=')
			j++;
		splited[0] = ft_strdup_until_i(envp[i], j);
		if (!splited[0])
			return (perror("ft_init_envlst failed"), FAILURE);
		splited[1] = ft_strdup_until_i(envp[i] + j + 1, ft_strlen(envp[i] + j));
		if (!splited[1])
			return (free(splited[0]), perror("ft_init_envlst failed"), FAILURE);
		if (new_var(msh, splited) == FAILURE)
			return (free(splited[0]), free(splited[1]), FAILURE);
		i++;
	}
	return (SUCCESS);
}
