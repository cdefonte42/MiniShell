/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 17:45:23 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/22 17:55:19 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"
//#include "varenv.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

int	ft_init_envlst(t_minishell *msh, char **envp)
{
	int			i;
	char		*splited[2];
	t_var_type	type;
	int			j;

	i = 0;
	ft_memset(splited, 0, sizeof(char *) * 2);
	if (!envp)
		return (FAILURE);
	while (envp && envp[i])
	{
		type = envvar;
		j = 0;
		while (envp[i] && envp[i][j] != '=')
			j++;
		splited[0] = ft_strdup_until_i(envp[i], j);
		if (!splited[0])
			return (perror("ft_init_envlst failed"), FAILURE);
		splited[1] = ft_strdup_until_i(envp[i] + j + 1, ft_strlen(envp[i] + j));
		if (!splited[1])
			return (free(splited[0]), perror("ft_init_envlst failed"), FAILURE);
		if (ft_strcmp(splited[0], "_") == 0)
			type = shellvar;
		if (ft_new_var(&(msh->vars), splited[0], splited[1], type) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
