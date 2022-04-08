/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:04:56 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/08 16:46:16 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"
#include "export.h"

/* Print le message d'erreur specifique a 'cd'. Peut etre change en return int
pour return exit status = 128 + errno */
static void	ft_put_error(char *token)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("export: `", 2);
	if (token && *token)
	{
		ft_putstr_fd(token, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
}

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
	*new_var = (t_var){.key = key, .value = value, .type = type};
	var_add_back(var_lst, new_var);
	return (SUCCESS);
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

/* Print un element t_var selon le modele du bin export sans arg */
void	ft_print_export(t_var *lst)
{
	if (!lst || lst->type)
		return ;
	if (lst->value == NULL)
		printf("export %s\n", lst->key);
	else
		printf("export %s=\"%s\"\n", lst->key, lst->value);
}

int	ft_count_vars(t_var *var_lst)
{
	t_var	*head;
	int		i;

	i = 0;
	head = var_lst;
	// Check if is export var and not shell var
	while (head)
	{
		head = head->next; 
		++i;
	}
	return (i);
}

/* Print les var d'env (et pas shell var) de la liste var_lst au format 
d'export cad ds ordre alpha*/
int	ft_put_export(t_var *var_lst)
{
	t_var	*curr_kmin;
	int		end;

	end = ft_count_vars(var_lst);
	if (!var_lst)
		return (0);
	curr_kmin = ft_get_minkey(var_lst);
	while (end--)
	{
		if (curr_kmin->type == envvar)
			ft_print_export(curr_kmin);
		// curr_kmin = ft_get_nextbigger(var_lst, curr_kmin, &end);
		curr_kmin = ft_get_minkey_prev(var_lst, curr_kmin);
	}
	return (0);
}

int	ft_loop_export(t_var **var_lst, char *token)
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
	while (token[key_len] && token[key_len] != '=')
		key_len++;
	if (key_len == 0)
		return (ft_put_error(token), -1);
	if ((int)ft_strlen(token) >= key_len + 1)
	{
		value = ft_strdup(token + key_len + 1);
		if (!value)
			return (perror("strdup export"), -1);
		if (token[key_len - 1] == '+')
		{
			key_len--;
			add_mode = 1;
		}
	}
	key = ft_substr(token, 0, key_len);
	if (!key)
		return (free(value), perror("substr export"), -1);
	if (*key == '_' && key_len == 1)
		return (free(value), free(key), 0);
	if (!ft_isname(key))
	{
		free(value);
		free(key);
		return (ft_put_error(token), -1);
	}
	var_exists = var_getfromkey(*var_lst, key);
	if (!var_exists)
	{
		if (ft_new_var(var_lst, key, value, envvar) == FAILURE)
			return (free(key), free(value), FAILURE);
	}
	else if (value != NULL)
	{
		if (add_mode)
			ft_cat_var(var_exists, key, value);
		else
		{
			free(var_exists->key);
			free(var_exists->value);
			var_exists->key = key;
			var_exists->value = value;
			var_exists->type = envvar;
		}
	}
	return (0);
}

int	ft_export(t_var **var_lst, char **argv)
{
	int		i;

	i = 1;
	if (!argv || !*argv || !argv[1])
	{
		ft_put_export(*var_lst);
		return (0);
	}
	while (argv[i])
	{
		ft_loop_export(var_lst, argv[i]);
		i++;
	}
	return (0);
}

// void	ft_print_lst(t_var *lst)
// {
// 	if (!lst)
// 		return ;
// 	while (lst)
// 	{
// 		printf("name=%s value=%s\n", lst->key, lst->value);
// 		lst = lst->next;
// 	}
// }
//
//
// int	main(void)
// {
// 	t_var	*var;
//
// 	var = NULL;
// 	ft_export(&var, "B=LALALALA");
// 	ft_export(&var, "C=poulet");
// 	ft_export(&var, "A=nieh");
// 	ft_export(&var, "D=""");
// 	ft_export(&var, "E=");
// 	ft_export(&var, NULL);
// 	ft_lst_clear(var);
// 	return (0);
// }
