/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/30 21:57:37 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/11 11:51:09 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
#include "minishell.h"
#include "libft.h"

extern int	g_status;

int	rand_hdname(t_cmde *cmd_lst)
{
	unsigned long	ptr;
	char			*tmppath;
	char			*tmp;

	tmppath = "/tmp/";
	ptr = (unsigned long) cmd_lst;
	if (cmd_lst->hdfile)
		return (SUCCESS);
	tmp = ft_itoa(ptr);
	if (!tmp)
		return (FAILURE);
	cmd_lst->hdfile = ft_strjoin(tmppath, tmp);
	free(tmp);
	if (!cmd_lst->hdfile)
		return (FAILURE);
	return (SUCCESS);
}

int	get_value_hd(char *dolls, char **value, t_var *var_lst)
{
	char	*find;

	if (!dolls)
		return (SUCCESS);
	if (*(dolls + 1) == '?')
	{
		*value = ft_itoa(g_status);
		if (!*value)
			return (FAILURE);
		return (SUCCESS);
	}
	find = var_getvaluefromkey(var_lst, dolls + 1);
	*value = ft_strdup(find);
	if (!*value)
		return (FAILURE);
	return (SUCCESS);
}

int	expand_hdstr(char **str, t_var *var_lst)
{
	int		i;
	char	*dolls;
	char	*value;

	i = 0;
	while (str && *str && (*str)[i])
	{
		dolls = NULL;
		value = NULL;
		if ((*str)[i] == '$' && (*str)[i + 1])
		{
			if (get_dolls(*str + i, &dolls) == FAILURE
				|| get_value_hd(dolls, &value, var_lst) == FAILURE
				|| exp_replacewith(str, i, dolls, value) == FAILURE)
				return (free(dolls), free(value), FAILURE);
			if (!dolls && (!value || !*value))
				i += 1;
			i += ft_strlen(value);
			free(dolls);
			free(value);
		}
		else
			i++;
	}
	return (SUCCESS);
}
