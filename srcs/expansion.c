/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 14:10:15 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/30 18:12:50 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

extern int	g_status;

int	expand_str(char **str, t_var *var_lst)
{
	int		i;
	int		inquote;
	char	*dolls;
	char	*value;

	i = 0;
	inquote = nil;
	while (str && *str && (*str)[i])
	{
		dolls = NULL;
		value = NULL;
		set_curr_quote((*str)[i], &inquote);
		if (inquote != singleq && (*str)[i] == '$' && (*str)[i + 1])
		{
			if (get_dolls(*str + i, &dolls) == FAILURE)
				return (FAILURE);
			if (get_value(dolls, &value, var_lst, inquote) == FAILURE)
				return (free(dolls), FAILURE);
			if (exp_replacewith(str, i, dolls, value) == FAILURE)
				return (free(dolls), free(value), FAILURE);
			i += ft_strlen(value);
			free(dolls);
			free(value);
		}
		else
			i++;
	}
	return (SUCCESS);
}

int	ft_expand_tokens(t_cmde *cmd, t_var *vars_lst)
{
	t_token	*head_token;

	if (!cmd || !cmd->cmde_line)
		return (SUCCESS);
	head_token = cmd->cmde_line;
	while (head_token)
	{
		if (head_token->type == word && ft_strchr(head_token->str, '$'))
		{
			if (expand_str(&(head_token->str), vars_lst) == FAILURE)
				return (FAILURE);
		}
		head_token = head_token->next;
	}
	ft_remove_empty_token(&cmd->cmde_line);
	return (SUCCESS);
}
