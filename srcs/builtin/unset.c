/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 13:56:58 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/04 12:06:36 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Print le message d'erreur specifique a 'cd'. Peut etre change en return int
pour return exit status = 128 + errno */
static void	ft_put_error(char *token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("unset: `", 2);
	if (token && *token)
	{
		ft_putstr_fd(token, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
}

/* Remove et free l'element (et ses elements 'key' et 'value') de la liste 
'lst' ayant comme key 'key' */
void	ft_rm_varlst_elem(t_var **lst, char *key)
{
	t_var	*to_rm;
	t_var	*prev;
	t_var	*elem;

	if (!lst)
		return ;
	elem = *lst;
	prev = NULL;
	while (elem)
	{
		if (ft_strcmp(elem->key, key) == 0)
		{
			to_rm = elem;
			free(to_rm->key);
			free(to_rm->value);
			if (prev)
				prev->next = to_rm->next;
			else
				*lst = to_rm->next;
			free(to_rm);
			return ;
		}
		prev = elem;
		elem = elem->next;
	}
}

int	ft_unset(t_var **lst, char **argv)
{
	int		i;
	int		status;

	status = 0;
	i = 1;
	if (!argv)
		return (0);
	while (argv[i])
	{
		ft_rm_varlst_elem(lst, argv[i]);
		if (!ft_isname(argv[i]))
		{
			ft_put_error(argv[i]);
			status = 1;
		}
		i++;
	}
	return (status);
}
