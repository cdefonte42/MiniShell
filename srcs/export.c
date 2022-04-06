/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:04:56 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/06 13:21:54 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_back(t_var **alst, t_var *new)
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

t_var	*var_getfromkey(t_var *var_list, char *key)
{
	t_var	*head;

	if (var_list == NULL)
		return (NULL);
	head = var_list;
	while (head && ft_strcmp(head->key, key) != 0)
		head = head->next;
	return (head);
}

int	ft_set_var(t_var **var_lst, char *str)
{

}

/* Export la variable declaree dans str avec sa value si str contient '=' */
int	ft_add_var(t_var **var_lst, char *str)
{
	t_var	*new_var;
	t_var	*old_var;
	char	**value;
	char	*new;
	
	value = ft_split(str, '=');
	if (!value)
		return (-1);
	old_var = var_getfromkey(*var_lst, value[0]);
	if (old_var == NULL)
	{
		new_var = malloc(sizeof(t_var));
		if (!new_var)
			return (free(value), free(value[0]), free(value[1]), -1);
		*new_var = (t_var) {.key = value[0], .value = value[1]};
		ft_add_back(var_lst, new_var);
	}
	else
	{
		new = ft_strdup(old_var->value);
	}
	free(value);
	return (0);
}

void	ft_export(t_var **var_lst, char *str)
{
	if (!str)
	{
		// PRINT
		return ;;
	}
}

int	main(void)
{
	t_var	*list;

	list = NULL;
	ft_add_var(&list, "PROUT= ");
	printf("%s=%s\n", list->key, list->value);
}
