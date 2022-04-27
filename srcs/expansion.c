/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 14:10:15 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/27 14:02:16 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

extern int g_status;

static int	get_start(t_token *token, int start, t_quote_type *iq)
{
	while (token->str[start] && (*iq == singleq || token->str[start] != '$'))
	{
		if (token->str[start] == '"' && (*iq != singleq || *iq == 0))
		{
			*iq = *iq ^ doubleq;
			token->qtype = doubleq;
		}
		else if (token->str[start] == '\'' && (*iq != doubleq || *iq == 0))
		{
			*iq = *iq ^ singleq;
			token->qtype = singleq;
		}
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
	if (dolls[len - 1] != '?')
		value = var_getvaluefromkey(vars_lst, dolls + 1);
	else
	{
		value = ft_itoa(g_status);
		if (!value)
			return (FAILURE);
	}
	newstr = ft_replacestr_i(start, token->str, dolls, value);
	if (dolls[len - 1] == '?')
		free(value);
	free(dolls);
	if (!newstr)
		return (FAILURE);
	free(token->str);
	token->str = newstr;
	return (SUCCESS);
}

static int	replace_bis(t_token *token, t_var *vars_lst, int start, int len)
{
	char	*dolls;
	char	*value;
	char	*tmp;
	char	*newstr;
	char	*getval;

	dolls = ft_substr(token->str, start, len);
	if (!dolls)
		return (FAILURE);
	if (dolls[len - 1] != '?')
	{
		getval = var_getvaluefromkey(vars_lst, dolls + 1);
		if (!getval)
			return (SUCCESS);
		value = ft_strdup(getval);
		if (!value)
			return (FAILURE);
		if (msh_isquoted(value))
		{
			tmp = value;
			value = ft_strsjoin("\"", tmp, "\"");
			if (!value)
				return (free(tmp), FAILURE);
		}
	}
	else
	{
		value = ft_itoa(g_status);
		if (!value)
			return (FAILURE);
	}
	newstr = ft_replacestr_i(start, token->str, dolls, value);
	free(value);
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
	if ((token->str[start] == '$' && !(ft_fcisname(token->str[start + 1]) ||\
	token->str[start + 1] == '?')))
		return (ft_expand_token(token, vars_lst, start + 1, iq));
	while (token->str[start + len - 1] != '?' &&\
	ft_cisname(token->str[start + len]))
		len++;
	if (iq == doubleq && replace(token, vars_lst, start, len) == FAILURE)
		return (FAILURE);
	else if (iq == nil && replace_bis(token, vars_lst, start, len) == FAILURE)
		return (FAILURE);
	return (ft_expand_token(token, vars_lst, start, iq));
}

/*Remove de la token_lst les elements dont la str est NULL ou str[0] == NULL */
void	ft_remove_empty_token(t_token **token_lst)
{
	t_token	*head;
	int		i;

	head = *token_lst;
	i = 0;
	while (head)
	{
		if (head->str == NULL || head->str[0] == 0)
		{
			ft_tokenlst_pop(token_lst, i);
			head = *token_lst;
			i = 0;
		}
		else
		{
			i++;
			head = head->next;
		}
	}
}

/* L'expansion est faite APRES les pipes ET APRES les redirections */
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
	ft_remove_empty_token(&cmde_elem->cmde_line);
	return (SUCCESS);
}
