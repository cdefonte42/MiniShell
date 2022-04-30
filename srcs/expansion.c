/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 14:10:15 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/29 18:07:41 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

extern int	g_status;

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
			return (free(dolls), FAILURE);
	}
	newstr = ft_replacestri(start, token->str, dolls, value);
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
			return (free(dolls), FAILURE);
		if (msh_isquoted(value))
		{
			tmp = value;
			value = ft_strsjoin("\"", tmp, "\"");
			free(tmp);
			if (!value)
				return (free(dolls), free(value), FAILURE);
		}
	}
	else
	{
		value = ft_itoa(g_status);
		if (!value)
			return (free(dolls), FAILURE);
	}
	newstr = ft_replacestri(start, token->str, dolls, value);
	if (dolls[len - 1] == '?')
		free(value);
	free(dolls);
	if (!newstr)
		return (FAILURE);
	free(token->str);
	token->str = newstr;
	return (SUCCESS);
}

int	ft_expand_token(t_token *token, t_var *var_lst, int start, int qtype)
{
	int		len;

	len = 2;
	if (!token || !token->str || !token->str[start])
		return (SUCCESS);
	start = get_start(token, start, &qtype);
	if (!token->str[start])
		return (SUCCESS);
	if ((token->str[start] == '$' && !(ft_fcisname(token->str[start + 1]) || \
	token->str[start + 1] == '?')))
		return (ft_expand_token(token, var_lst, start + 1, qtype));
	while (token->str[start + len - 1] != '?' && \
	ft_cisname(token->str[start + len]))
		len++;
	if (qtype == doubleq && replace(token, var_lst, start, len) == FAILURE)
		return (FAILURE);
	else if (qtype == nil && replace_bis(token, var_lst, start, len) == FAILURE)
		return (FAILURE);
	return (ft_expand_token(token, var_lst, start, qtype));
}

int	ft_expansion(t_cmde *cmd, t_var *vars_lst)
{
	t_token	*head_token;

	if (!cmd || !cmd->cmde_line)
		return (SUCCESS);
	head_token = cmd->cmde_line;
	while (head_token)
	{
		if (head_token->type == word && ft_strchr(head_token->str, '$'))
		{
			if (ft_expand_token(head_token, vars_lst, 0, nil) == FAILURE)
				return (FAILURE);
		}
		head_token = head_token->next;
	}
	ft_remove_empty_token(&cmd->cmde_line);
	return (SUCCESS);
}
