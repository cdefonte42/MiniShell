/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 15:05:59 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/27 15:47:56 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	ft_pwd(int fdout)
{
	char	*pwd;

	errno = 0;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("perror cd");
		return (1);
	}
	ft_putstr_fd(pwd, fdout);
	ft_putstr_fd("\n", fdout);
	free(pwd);
	return (0);
}
