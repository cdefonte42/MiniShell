/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:14:35 by mbraets           #+#    #+#             */
/*   Updated: 2022/05/04 11:52:40 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"
#include "vars_lst.h"

/* Retourne le nombre d'elements dans la liste 'var_lst' dont les values sont
NON NULLES*/
int	ft_varlst_size_empty(t_var *var_lst)
{
	t_var	*head;
	int		i;

	i = 0;
	head = var_lst;
	while (head)
	{
		if (head->value)
			++i;
		head = head->next;
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

/* Retourne le ptr de la valeur correspondant à la clé `key`. */
char	*var_getvaluefromkey(t_var *var_list, char *key)
{
	t_var	*head;

	if (var_list == NULL || !key)
		return (NULL);
	head = var_list;
	while (head != NULL && head->key != NULL && ft_strcmp(head->key, key) != 0)
		head = head->next;
	if (head == NULL)
		return (NULL);
	return (head->value);
}

/* Concatenation de la current value de l'element 'var' ayant pour key valeur
'key', avec le prama 'value'. Return FAILURE si erreur de mallo.*/
int	ft_cat_var(t_var *var, char *key, char *value)
{
	char	*newvalue;
	int		len_new;
	int		len_old;

	len_new = ft_strlen(value);
	len_old = 0;
	if (var->value)
		len_old = ft_strlen(var->value);
	if (ft_palloc(&newvalue, sizeof(char) * (len_new + len_old + 1)))
		return (FAILURE);
	if (var->value)
		ft_strlcpy(newvalue, var->value, len_old + 1);
	ft_strlcat(newvalue, value, len_old + len_new + 1);
	free(var->value);
	free(value);
	free(key);
	var->value = newvalue;
	return (SUCCESS);
}
