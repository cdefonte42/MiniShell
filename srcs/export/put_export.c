/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 10:40:11 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/04 12:01:25 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vars_lst.h"
#include "export.h"

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

/* Print un element t_var selon le modele de export sans arg */
void	ft_print_export(t_var *lst, int fdout)
{
	if (!lst || lst->type)
		return ;
	if (lst->type == shellvar)
		return ;
	ft_putstr_fd("export ", fdout);
	ft_putstr_fd(lst->key, fdout);
	if (lst->value)
	{
		ft_putstr_fd("=\"", fdout);
		ft_putstr_fd(lst->value, fdout);
		ft_putstr_fd("\"", fdout);
	}
	ft_putstr_fd("\n", fdout);
}

/* Print les var d'env (et pas shell var) de la liste var_lst au format 
d'export cad ds ordre alpha*/
int	ft_put_export(t_var *var_lst, int fdout)
{
	t_var	*curr_kmin;
	int		end;

	end = ft_varlst_size(var_lst);
	if (!var_lst)
		return (0);
	curr_kmin = ft_get_minkey(var_lst);
	while (end--)
	{
		if (curr_kmin->type == envvar)
			ft_print_export(curr_kmin, fdout);
		curr_kmin = ft_get_minkey_prev(var_lst, curr_kmin);
	}
	return (0);
}
