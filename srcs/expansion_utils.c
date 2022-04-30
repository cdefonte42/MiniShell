/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 18:02:37 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/30 18:05:50 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

extern int	g_status;

/* Modifie la valeur de qtype en fonction de si le charactere present est une 
quote et quel type. */
void	set_curr_quote(char c, int *qtype)
{
	if (c == '"' && (*qtype != singleq || *qtype == 0))
		*qtype = *qtype ^ doubleq;
	else if (c == '\'' && (*qtype != doubleq || *qtype == 0))
		*qtype = *qtype ^ singleq;
}

/* Remplace dans src 't_rep', a partir dee l'index start, 
par la chaine rep_w. Retourne FAILURE en cas de malloc error. Si src = $prout 
et rep_w = NULL, le resultat donne une str[0] = 0; */
int	exp_replacewith(char **src, int start, char *t_rep, char *rep_w)
{
	int		len_final;
	char	*final;
	int		len_totrim;

	if (!*src || !t_rep)
		return (SUCCESS);
	len_totrim = ft_strlen(t_rep);
	len_final = ft_strlen(*src) - len_totrim + ft_strlen(rep_w);
	final = ft_calloc(len_final + 1, sizeof(char));
	if (!final)
		return (FAILURE);
	ft_strlcpy(final, *src, start + 1);
	if (rep_w && *rep_w)
		ft_strlcat(final, rep_w, len_final + 1);
	ft_strlcat(final, *src + start + len_totrim, len_final + 1);
	free(*src);
	*src = final;
	return (SUCCESS);
}

int	get_dolls(char *str, char **dolls)
{
	int		len;

	if (!str || str[0] != '$')
		return (SUCCESS);
	len = 2;
	if (ft_fcisname(str[1]))
		while (ft_cisname(str[len]))
			len++;
	else if (str[1] != '?')
		return (SUCCESS);
	*dolls = ft_substr(str, 0, len);
	if (!*dolls)
		return (FAILURE);
	return (SUCCESS);
}

int	get_value(char *dolls, char **value, t_var *var_lst, int inquote)
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
	if (inquote == nil && msh_isquoted(find))
		*value = ft_strsjoin("\"", find, "\"");
	else
		*value = ft_strdup(find);
	if (!*value)
		return (FAILURE);
	return (SUCCESS);
}
