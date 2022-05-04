/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:45:33 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/04 12:08:05 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vars_lst.h"
#include "libft.h"
#include <stdlib.h>

#define FAILURE 0
#define SUCCESS 1

/* Ajoute l'element de strucutre t_var 'new' a la fin de la liste 'alst' */
void	var_add_back(t_var **alst, t_var *new)
{
	t_var	*last;

	if (alst == NULL || new == NULL)
		return ;
	if (*alst == NULL)
	{
		*alst = new;
		return ;
	}
	last = *alst;
	while (last->next)
		last = last->next;
	last->next = new;
}

/* Cree et ajoute une nouvel element a la fin de la list var_lst.
Les arguments key et value doivent avoir ete declare dans la heap avant appel.
Retorune 1 en cas d'erreur de malloc, et free key et value. */
int	ft_new_var(t_var **var_lst, char *key, char *value, int type)
{
	t_var	*new_var;

	new_var = ft_calloc(1, sizeof(t_var));
	if (!new_var)
		return (FAILURE);
	new_var->key = key;
	new_var->value = value;
	new_var->type = type;
	var_add_back(var_lst, new_var);
	return (SUCCESS);
}

/* Free les valeurs actuelles de key et value de l'elem var. Remplace par ceux
passes en param (doivent etre alloues). */
void	ft_set_var(t_var *var, char *key, char *value, int type)
{
	free(var->key);
	free(var->value);
	var->key = key;
	var->value = value;
	var->type = type;
}

/* Free tous les elements de struct t_var et la liste */
void	ft_varlst_clear(t_var *lst)
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

/* Retourne le nombre d'elements dans la liste 'var_lst' */
int	ft_varlst_size(t_var *var_lst)
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
