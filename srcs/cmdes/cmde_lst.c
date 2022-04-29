/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmde_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:00:55 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 11:17:31 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdes.h"
#include "tokens.h"
#include <stdlib.h>
#include <stdio.h>

t_cmde	*ft_cmdelst_new(t_token *cmde_line)
{
	t_cmde	*new_cmde;
	
	new_cmde = NULL;
	new_cmde = malloc(sizeof(t_cmde));
	if (!new_cmde)
		return (NULL);
	new_cmde->cmde_line = cmde_line;
	new_cmde->pipefd[r_end] = 0;
	new_cmde->pipefd[w_end] = 1;
	new_cmde->pid = -1;
	new_cmde->hdfile = NULL;	
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

/* Free le CONTENU et les ELEMENTS de la liste de cmdes 'lst'*/ 
void	ft_cmdelst_clear(t_cmde *lst)
{
	t_cmde	*last;

	last = lst;
	while (lst)
	{
		last = lst;
		lst = lst->next;
		ft_tokenlst_free(last->cmde_line);
		free(last->hdfile);
		last->hdfile = NULL;
		free(last);
		last = NULL;
	}
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
