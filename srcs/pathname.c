/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 10:31:13 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/26 12:54:29 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

char	**ft_split_paths(t_minishell *msh)
{
	char	**paths;
	char	*var;

	var = var_getvaluefromkey(msh->vars, "PATH");
	if (!var)
		return (NULL);
	paths = ft_split(var, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

static char	*get_cmd(t_minishell *msh, char *cmd)
{
	char	*temp;
	char	*command;
	char	**paths;
	int		i;

	paths = ft_split_paths(msh);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths && paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		command = ft_strjoin(temp, cmd);
		free(temp);
		if (access(command, 0) == 0)
			return (command);
		free(command);
		i++;
	}
	ft_free_tabtab(paths);
	return (NULL);
}

char	*check_permission(t_minishell *msh, char *cmd)
{
	if ((*cmd == '.' || *cmd == '/'))
	{
		if (access(cmd, 0) == 0)
		{
			if (access(cmd, R_OK | X_OK) == 0)
				return (ft_strdup(cmd));
			else
				return (NULL);
		}
		else
			return (NULL);
	}
	else if (!cmd || *cmd == 0)
	{
		return (errno = 2, NULL);
	}
	else
		return (get_cmd(msh, cmd));
	return (NULL);
}
