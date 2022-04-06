/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:04:56 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/06 17:09:07 by mbraets          ###   ########.fr       */
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

t_var	*var_getfromkey(t_var **var_list, char *key)
{
	t_var	*head;

	if (var_list == NULL || *var_list == NULL)
		return (NULL);
	head = *var_list;
	while (head != NULL && key && head->key != NULL && ft_strcmp(head->key, key) != 0)
		head = head->next;
	return (head);
}


/* Export la variable declaree dans str avec sa value si str contient '=' */
int	ft_new_var(t_var **var_lst, char *name, char *value)
{
	t_var	*new_var;

	new_var = malloc(sizeof(t_var));
	if (!new_var)
		return (free(value), free(name), -1);
	*new_var = (t_var) {.key = &value[0], .value = &value[1]};
	ft_add_back(var_lst, new_var);
	free(value);
	return (0);
}

int	ft_cat_var(t_var *var, char *value)
{
	char	*newvalue;
	int		len_new;
	int		len_old;

	len_new = ft_strlen(value);
	len_old = ft_strlen(var->value);
	if (ft_palloc(&newvalue, sizeof(char) * (len_new + len_old)))
		return (FAILURE);
	ft_strlcpy(newvalue, var->value, len_old);
	ft_strlcat(newvalue, value, len_new);
	free(var->value);
	free(value);
	var->value = newvalue;
	return (SUCCESS);
}

int	ft_export(t_var **var_lst, char *str)
{
	char	*name;
	char	*value;
	int		add_mode;
	int		name_len;

	add_mode = 0;
	name_len = 0;
	if (!str)
	{
		// PRINT
		return (0);
	}
	while (str[name_len] && str[name_len] != '=')
		name_len++;
	if (name_len == 0)
		return (ft_putstr_fd("export: not an identifier: str HANDLER\n", 2), -1);
	if ((int)ft_strlen(str) >= name_len + 1)
	{
		value = ft_strdup(str + name_len + 1);
		if (!value)
			return (perror("strdup export"), -1);
	}
	if (str[name_len - 1] == '+')
	{
		name_len--;
		add_mode = 1;
	}
	name = ft_substr(str, 0, name_len);
	if (!name)
		return (free(value), perror("substr export"), -1);
	if (*name == '_' && name_len == 1)
		return (free(value), free(name), 0);
	if (!ft_isname(name))
	{
		free(value);
		free(name);
		return (ft_putstr_fd("export: not an identifier: str HANDLER\n", 2), -1);
	}
	if (var_getfromkey(var_lst, name) == NULL)
		ft_new_var(var_lst, name, value);
	else
		if (add_mode)
			ft_cat_var(var_getfromkey(var_lst, name), value);
		// else
			// ft_set_var(var_lst, name, value);
	return (0);
}

int	main(void)
{
	t_var *var;

	if (ft_palloc(&var, sizeof(t_var)))
		return (1);
	ft_export(&var, "PROUT==idsdsd");
	ft_export(&var, "PROUT+=dsdsd");
	return (0);
}
