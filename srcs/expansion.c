/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 14:10:15 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/26 12:04:17 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

static int	get_start(t_token *token, int start, t_quote_type *iq)
{
	while (token->str[start] && (*iq == singleq || token->str[start] != '$'))
	{
		if (token->str[start] == '"' && (*iq != singleq || *iq == 0))
			*iq = *iq ^ doubleq;
		else if (token->str[start] == '\'' && (*iq != doubleq || *iq == 0))
			*iq = *iq ^ singleq;
		start++;
	}
	return (start);
}

static int	replace(t_token *token, t_var *vars_lst, int start, int len)
{
	char	*dolls;
	char	*value;
	char	*newstr;

	dolls = ft_substr(token->str, start, len);
	if (!dolls)
		return (FAILURE);
	value = var_getvaluefromkey(vars_lst, dolls + 1);
	newstr = ft_replacestr_i(start, token->str, dolls, value);
	free(dolls);
	if (!newstr)
		return (FAILURE);
	free(token->str);
	token->str = newstr;
	return (SUCCESS);
}

int	ft_expand_token(t_token *token, t_var *vars_lst, int start, t_quote_type iq)
{
	int		len;

	len = 2;
	if (!token || !token->str || !token->str[start])
		return (SUCCESS);
	start = get_start(token, start, &iq);
	if (!token->str[start])
		return (SUCCESS);
	if (token->str[start] == '$' && !token->str[start + 1] \
	&& !ft_fcisname(token->str[start + 1]))
		return (ft_expand_token(token, vars_lst, start + 1, iq));
	while (ft_cisname(token->str[start + len]))
		len++;
	if (replace(token, vars_lst, start, len) == FAILURE)
		return (FAILURE);
	return (ft_expand_token(token, vars_lst, start, iq));
}

/* L'expansion est faite APRES les pipes ET APRES les redirections */
/* AUTRE pb: il peut y avoir PLUSIEURS $var dans un seul token! */
int	ft_expansion(t_cmde *cmde_elem, t_var *vars_lst)
{
	t_token	*head_token;

	if (!cmde_elem || !cmde_elem->cmde_line)
		return (SUCCESS);
	head_token = cmde_elem->cmde_line;
	while (head_token)
	{
		if (head_token->type == word && ft_strchr(head_token->str, '$'))
		{
			if (ft_expand_token(head_token, vars_lst, 0, nil) == FAILURE)
				return (FAILURE);
		}
		head_token = head_token->next;
	}
	return (SUCCESS);
}
