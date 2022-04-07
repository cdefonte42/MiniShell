/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:14:35 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/07 15:26:23 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

/* Retourne l'element de la liste 'var_list' qui a comme key valeur 'key'. */
t_var	*var_getfromkey(t_var *var_list, char *key)
{
	t_var	*head;

	if (var_list == NULL || !key)
		return (NULL);
	head = var_list;
	while (head != NULL && head->key != NULL && ft_strcmp(head->key, key) != 0)
		head = head->next;
	return (head);
}

/* Retourne l'element de la liste var_lst qui a la plus petite valeur ASCII 
pour son element de struct t_var 'key' */
t_var	*ft_get_minkey(t_var *var_lst)
{
	t_var	*min;

	if (!var_lst)
		return (NULL);
	min = var_lst;
	while (var_lst)
	{
		if (ft_strcmp(min->key, var_lst->key) > 0)
			min = var_lst;
		var_lst = var_lst->next;
	}
	return (min);
}

/* Retourne l'element dans la liste plsu grand que la valeur ASCII de de la key
de 'prevmin'. ATTENTION prevmin ne doit PAS etre NULL, donc necessite d'avoir 
trouve le plus petit element ds la liste avant appel. */
t_var	*ft_get_nextbigger(t_var *lst, t_var *prevmin, int *end)
{
	t_var	*currmin;
	int		currdiff;
	int		diff;

	currdiff = 500;
	*end = 1;
	if (!prevmin)
		return (NULL);
	while (lst)
	{
		diff = ft_strcmp(lst->key, prevmin->key);
		if (diff > 0 && diff < currdiff)
		{
			*end = 0;
			currdiff = diff;
			currmin = lst;
		}
		lst = lst->next;
	}
	return (currmin);
}

/* Free tous les elements de struct t_var et la liste */
void	ft_lst_clear(t_var *lst)
{
	t_var	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
