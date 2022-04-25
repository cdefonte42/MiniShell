/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 10:31:13 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/25 10:53:32 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char	**ft_split_paths(t_minishell *msh)
{
	char	**paths;
	char	*val;

	var = ft_getvaluefromkey(msh->vars, "PATHS");
	if (!var)
		return (NULL);
	paths = ft_split(var, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

static char    *get_cmd(t_minishell *msh, char *cmd)
{
    char    *temp;
    char    *command;
	char	**paths;

	paths = ft_split_paths(msh);
	if (!paths)
		return (NULL);
    while (paths && *paths)
    {
        temp = ft_strjoin(*paths, "/");
        command = ft_strjoin(temp, cmd);
        free(temp);
        if (access(command, 0) == 0)
            return (command);
        free(command);
        paths++;
    }
    return (NULL);
}

char    *check_permission(t_minishell *msh, char *cmd)
{
    if ((*cmd == '.' || *cmd == '/'))
    {
        if (access(cmd, 0) == 0)
        {
            if (access(cmd, R_OK | X_OK) == 0)
                return (ft_strdup(cmd));
            else
            {
                ft_error(cmd, ERR_PERM);
				ft_msh_clear(msh);
                exit(126);
            }
        }
        else
            return (NULL);
    }
    else
        return (get_cmd(msh, cmd));
    return (NULL);
}

