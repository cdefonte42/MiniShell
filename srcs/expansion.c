/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 14:10:15 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/23 14:06:42 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

int	ft_expand_token(t_token *token, t_var *vars_lst, int start, t_quote_type inquote)
{
	char			*dolls;
	char			*value;
	int				len_var;
	char			*newstr;

	len_var = 2;
	if (!token || !token->str || !token->str[start])
		return (SUCCESS);
	while (token->str[start] && (inquote == singleq || token->str[start] != '$'))
	{
		if (token->str[start] == '"' && (inquote != singleq || inquote == 0))
		{
			inquote = inquote ^ doubleq;
		}
		else if (token->str[start] == '\'' && (inquote != doubleq || inquote == 0))
		{
			inquote = inquote ^ singleq;
		}
		start++;
	}
	if (!token->str[start])
		return (SUCCESS);
	else if (token->str[start] == '$' && !ft_fcisname(token->str[start + 1]))
		ft_expand_token(token, vars_lst, start + 1, inquote);
	while (ft_cisname(token->str[start + len_var]))
		len_var++;
	dolls = ft_substr(token->str, start, len_var);
	if (!dolls)
		return (FAILURE);
	value = var_getvaluefromkey(vars_lst, dolls + 1);
	newstr = ft_replacestr_i(start, token->str, dolls, value);
	free(dolls);
	if (!newstr)
		return (FAILURE);
	free(token->str);
	token->str = newstr;
	return (ft_expand_token(token, vars_lst, start, inquote));
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
