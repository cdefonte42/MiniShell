/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:14:35 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/08 16:53:50 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

/* Retourne le nombre d'elements dans la liste 'var_lst' */
int	ft_count_vars(t_var *var_lst)
{
	t_var	*head;
	int		i;

	i = 0;
	head = var_lst;
	while (head)
	{
		head = head->next; 
		++i;
	}
	return (i);
}

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

/*

	IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!
	For get_minkey_prev and get_minkey check the TYPE
		DONT PRINT THE SHELL VARIABLE
		
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

*/


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

t_var	*ft_get_maxkey(t_var *var_lst)
{
	t_var	*max;

	if (!var_lst)
		return (NULL);
	max = var_lst;
	while (var_lst)
	{
		if (ft_strcmp(max->key, var_lst->key) < 0)
			max = var_lst;
		var_lst = var_lst->next;
	}
	return (max);
}

t_var	*ft_get_minkey_prev(t_var *var_lst, t_var *prev)
{
	t_var	*min;

	if (!var_lst)
		return (NULL);
	min = ft_get_maxkey(var_lst);
	while (var_lst)
	{
		if (ft_strcmp(prev->key, var_lst->key) < 0)
			if (ft_strcmp(min->key, var_lst->key) > 0)
				min = var_lst;
		var_lst = var_lst->next;
	}
	return (min);
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
