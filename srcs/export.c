/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:04:56 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/06 17:38:58 by cdefonte         ###   ########.fr       */
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


/* Cree et ajoute une nouvel element a la fin de la list var_lst.
Les arguments key et value doivent avoir ete declare dans la heap avant appel.
Retorune -1 en cas d'erreur de malloc, et free key et value. */
int	ft_new_var(t_var **var_lst, char *key, char *value)
{
	t_var	*new_var;

	new_var = malloc(sizeof(t_var));
	if (!new_var)
		return (free(value), free(key), -1);
	*new_var = (t_var) {.key = key, .value = value};
	ft_add_back(var_lst, new_var);
	return (0);
}

int	ft_cat_var(t_var *var, char *value)
{
	char	*newvalue;
	int		len_new;
	int		len_old;

	len_new = ft_strlen(value);
	len_old = ft_strlen(var->value);
	if (ft_palloc(&newvalue, sizeof(char) * (len_new + len_old + 1)))
		return (FAILURE);
	ft_strlcpy(newvalue, var->value, len_old + 1);
	ft_strlcat(newvalue, value, len_old + len_new + 1);
	free(var->value);
	free(value);
	var->value = newvalue;
	return (SUCCESS);
}

int	ft_export(t_var **var_lst, char *str)
{
	char	*key;
	char	*value;
	int		add_mode;
	int		key_len;
	t_var	*var_exists;

	add_mode = 0;
	key_len = 0;
	key = NULL;
	value = NULL;
	if (!str)
	{
		// PRINT
		return (0);
	}
	//ft_get_varkey(str, &key, mode);
	while (str[key_len] && str[key_len] != '=')
		key_len++;
	if (key_len == 0)
		return (ft_putstr_fd("export: not an identifier: str HANDLER\n", 2), -1);
	if ((int)ft_strlen(str) >= key_len + 1)
	{
		value = ft_strdup(str + key_len + 1);
		if (!value)
			return (perror("strdup export"), -1);
	}
	if (str[key_len - 1] == '+')
	{
		key_len--;
		add_mode = 1;
	}
	key = ft_substr(str, 0, key_len);
	if (!key)
		return (free(value), perror("substr export"), -1);
	if (*key == '_' && key_len == 1)
		return (free(value), free(key), 0);
	if (!ft_isname(key))
	{
		free(value);
		free(key);
		return (ft_putstr_fd("export: not an identifier: str HANDLER\n", 2), -1);
	}
	var_exists = var_getfromkey(var_lst, key);
	if (!var_exists)
		ft_new_var(var_lst, key, value);
	else
		if (add_mode)
			ft_cat_var(var_exists, value);
		else
		{
			free(var_exists->key);
			free(var_exists->value);
			var_exists->key = key;
			var_exists->value = value;
		}
	return (0);
}

int	main(void)
{
	t_var *var;

	if (ft_palloc(&var, sizeof(t_var)))
		return (1);
	ft_export(&var, "PROUT=bonjour");
	printf("____AVANT____\nNAME=%s\nVALUE=%s\n", var->key, var->value);
	ft_export(&var, "PROUT+=pouet");
	printf("____APRES____\nNAME=%s\nVALUE=%s\n", var->key, var->value);
	free(var->key);
	free(var->value);
	free(var);
	return (0);
}
