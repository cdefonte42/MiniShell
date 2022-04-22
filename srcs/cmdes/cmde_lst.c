/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmde_lst.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:00:55 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/22 14:33:46 by mbraets          ###   ########.fr       */
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
	new_cmde->pipefd[in] = 0;
	new_cmde->pipefd[out] = 0;
	new_cmde->pid = 0;
	new_cmde->fdin = 0;
	new_cmde->fdout = 1;
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

	while (lst)
	{
		//free(lst->prev);
		//free(lst->cmde_line->str);
		//free(lst->cmde_line);
		last = lst;
		lst = lst->next;
		ft_tokenlst_free(last->cmde_line);
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
