/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmde_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:00:55 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/15 12:20:16 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdes.h"
#include <stdlib.h>

static void	ft_free_tabtab(char **tab)
{
	int		i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;	
	}
	free(tab);
}

t_cmde	*ft_cmdelst_new(t_token *cmde_line)
{
	t_cmde	*new_cmde;
	
	new_cmde = NULL;
	new_cmde = malloc(sizeof(t_cmde));
	if (!new_cmde)
		return (NULL);
	new_cmde->cmde_line = cmde_line;
	new_cmde->argv = NULL;
	new_cmde->pipe[0] = -1;
	new_cmde->pipe[1] = -1;
	new_cmde->prev = NULL;	
	new_cmde->next = NULL;	
	return (new_cmde);
}

t_cmde	*ft_cmdelst_last(t_cmde *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);	
}

void	ft_cmdelst_addback(t_cmde **alst, t_cmde *new)
{
	t_cmde	*last;

	if (alst == NULL || new == NULL)
		return ;
	if (*alst == NULL)
	{
		*alst = new;
		return ;
	}
	last = ft_cmdelst_last(*alst);
	last->next = new;
	new->prev = last;
}

void	ft_cmdelst_free(t_cmde *lst)
{
	t_cmde	*last;

	while (lst)
	{
		free(lst->prev);
		free(lst->cmde_line);
		ft_free_tabtab(lst->argv);
		last = lst;
		lst = lst->next;
	}
	free(last);
}

int	ft_cmdelst_size(t_cmde *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}
