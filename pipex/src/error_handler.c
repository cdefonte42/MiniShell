/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 10:10:19 by cdefonte          #+#    #+#             */
/*   Updated: 2022/02/23 15:14:14 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Print la string s until le char c si trouve, dans fd */
void	ft_putstr_until_fd(char *s, char c, int fd)
{
	int		i;

	i = 0;
	if (s == NULL)
		return ;
	while (s && s[i] && s[i] != c)
		i++;
	write(fd, s, i);
}

int	ft_exit_stat(int type)
{
	int		exit_status;

	exit_status = 1;
	if (type == command && errno == ESRCH)
		exit_status = 127;
	else if (type == command && errno == EACCES)
		exit_status = 126;
	return (exit_status);
}

/* strerror need to be ended with ":" and whitespace */
int	ft_error_handler(char *bcse_of, t_cmde **cmde, int type)
{
	int		exit_status;
	char	*str_error;

	exit_status = ft_exit_stat(type);
	str_error = strerror(errno);
	if (type == command && errno == ESRCH)
		str_error = "command not found";
	else if (type == command && errno == EACCES)
		str_error = "permission denied";
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(str_error, 2);
	if (bcse_of)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_until_fd(bcse_of, ' ', 2);
	}
	write(2, "\n", 1);
	if (*cmde)
		ft_lstclear(cmde);
	return (exit_status);
}
