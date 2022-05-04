/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 10:04:56 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/04 12:03:00 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vars_lst.h"
#include "export.h"

extern int	g_status;

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
	g_status = 2;
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
		return (free(*value), perror("substr export"), FAILURE);
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
		return (free(key), free(value), ft_put_error(token), SUCCESS);
	var_exists = var_getfromkey(*var_lst, key);
	if (!var_exists && ft_new_var(var_lst, key, value, envvar) == FAILURE)
		return (free(key), free(value), perror("export newvar"), FAILURE);
	else if (var_exists && value != NULL && add_mode == 1)
	{
		if (ft_cat_var(var_exists, key, value) == FAILURE)
			return (free(key), free(value), perror("export catvar"), FAILURE);
	}
	else if (var_exists && value != NULL && add_mode == 0)
		ft_set_var(var_exists, key, value, envvar);
	return (SUCCESS);
}

int	ft_export(t_var **var_lst, char **argv, int fdout)
{
	int		i;

	i = 1;
	if (!argv || !*argv || !argv[1])
	{
		ft_put_export(*var_lst, fdout);
		return (0);
	}
	while (argv[i])
	{
		if (ft_loop_export(var_lst, argv[i]) == FAILURE)
			return (perror("Loop export fail"), -1);
		i++;
	}
	return (g_status);
}
