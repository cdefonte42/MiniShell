/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:04:56 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/22 16:28:32 by mbraets          ###   ########.fr       */
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

/* Print un element t_var selon le modele de export sans arg */
void	ft_print_export(t_var *lst)
{
	if (!lst || lst->type)
		return ;
	if (lst->type == shellvar)
		return ;
	if (lst->value == NULL)
		printf("export %s\n", lst->key);
	else
		printf("export %s=\"%s\"\n", lst->key, lst->value);
}

/* Print les var d'env (et pas shell var) de la liste var_lst au format 
d'export cad ds ordre alpha*/
int	ft_put_export(t_var *var_lst)
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
			ft_print_export(curr_kmin);
		curr_kmin = ft_get_minkey_prev(var_lst, curr_kmin);
	}
	return (0);
}

int	ft_get_keynval(char **key, char **value, int *mode, char *token)
{
	int		key_len;

	key_len = 0;
	while (token[key_len] && token[key_len] != '=')
		key_len++;
	if (key_len == 0)
		return (ft_put_error(token), FAILURE);
	if (token[key_len] == '=')
	{
		*value = ft_strdup(token + key_len + 1);
		if (!*value)
			return (perror("strdup export"), FAILURE);
		if (token[key_len - 1] == '+')
		{
			key_len--;
			*mode = 1;
		}
	}
	*key = ft_substr(token, 0, key_len);
	if (!*key)
		return (free(value), perror("substr export"), FAILURE);
	return (SUCCESS);
}

int	ft_loop_export(t_var **var_lst, char *token)
{
	char	*key;
	char	*value;
	int		add_mode;
	t_var	*var_exists;

	add_mode = 0;
	key = NULL;
	value = NULL;
	if (ft_get_keynval(&key, &value, &add_mode, token) == FAILURE)
		return (FAILURE);
	if (*key == '_' && *(key + 1) == 0)
		return (free(value), free(key), SUCCESS);
	if (!ft_isname(key))
		return (free(key), free(value), ft_put_error(token), FAILURE);
	var_exists = var_getfromkey(*var_lst, key);
	if (!var_exists && ft_new_var(var_lst, key, value, envvar) == FAILURE)
		return (free(key), free(value), FAILURE);
	else if (var_exists && value != NULL && add_mode == 1)
		ft_cat_var(var_exists, key, value);
	else if (var_exists && value != NULL && add_mode == 0)
		ft_set_var(var_exists, key, value, envvar);
	return (SUCCESS);
}

int	ft_export(t_var **var_lst, char **argv)
{
	int		i;
	int		ret_stat;

	i = 1;
	ret_stat = 0;
	if (!argv || !*argv || !argv[1])
	{
		ft_put_export(*var_lst);
		return (0);
	}
	while (argv[i])
	{
		if (ft_loop_export(var_lst, argv[i]) == FAILURE)
			ret_stat = 1;
		i++;
	}
	return (ret_stat);
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
// 	ft_varlst_clear(var);
// 	return (0);
// }
