/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 14:10:15 by mbraets           #+#    #+#             */
/*   Updated: 2022/05/13 16:57:12 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

extern int	g_status;

static int	expand_add(char *dolls, char *value)
{
	int	i;

	i = 0;
	if ((!dolls && (!value || !*value)))
		i += 1;
	i += ft_strlen(value);
	return (i);
}

static int	expand_replace(int inquote, char **str, t_var *var_lst, int *i)
{
	char	*kval[2];

	kval[0] = NULL;
	kval[1] = NULL;
	if ((*str)[*i + 1] == '\'' || (*str)[*i + 1] == '"')
	{
		if (exp_replacewith(str, *i, "$", NULL) == FAILURE)
			return (FAILURE);
	}
	else if (get_dolls(*str + (*i), &kval[0]) == FAILURE
		|| get_value(kval[0], &kval[1], var_lst, inquote) == FAILURE
		|| exp_replacewith(str, *i, kval[0], kval[1]) == FAILURE)
		return (free(kval[0]), free(kval[1]), FAILURE);
	*i += expand_add(kval[0], kval[1]);
	free(kval[0]);
	free(kval[1]);
	return (SUCCESS);
}

/* Recoit un token et doit verifier s'il faut split a partir de l'index 'start'
jusqu'a l'index 'end' inclus. */
int	msh_split_token(t_token **token, int start, int end)
{
	int		i;
	char	*str;
	fprintf(stderr, "str to split =_%s_ start = %d end = %d\n", (*token)->str, start, end);

	str = (*token)->str;
	while (str && str[start] && start <= end && ft_isblank(str[start]))
		start++;
	return (SUCCESS);
}

int	expand_str(t_var *var_lst, t_token **token)
{
	int		i;
	int		i_before;
	int		inquote;

	i = 0;
	inquote = nil;
	while ((*token)->str && ((*token)->str)[i])
	{
		set_curr_quote(((*token)->str)[i], &inquote);
		if (inquote != singleq && ((*token)->str)[i] == '$' && ((*token)->str)[i + 1])
		{
			i_before = i;
			if (expand_replace(inquote, &((*token)->str), var_lst, &i) == FAILURE)
				return (FAILURE);
			if (inquote != doubleq && i > i_before && msh_split_token(token, i_before, i - 1) == FAILURE)
				return (FAILURE);
		}
		else
			i++;
	}
	return (SUCCESS);
}

//int	msh_split_token(t_token *token)
//{
//	t_token	*new_token;
//	char	**splitted;
//	int		i;
//
//	if (!token || !token->str || !token->str[0])
//		return (SUCCESS);
//	splitted = ft_split(token->str, ' ');
//	if (!splitted)
//		return (FAILURE);
//	free(token->str);
//	token->str = splitted[0];
//	i = 1;
//	while (splitted[i])
//	{
//		new_token = ft_tokenlst_new(splitted[i], word);
//		if (!new_token)
//			return (ft_free_tabtab(splitted), FAILURE);
//		ft_tokenlst_addafter(token, new_token);
//		token = token->next;
//		i++;
//	}
//	free(splitted);
//	return (SUCCESS);
//}

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
			if (expand_str(vars_lst, &head_token) == FAILURE)
				return (FAILURE);
		}
		head_token = head_token->next;
	}
	ft_remove_empty_token(&cmd->cmde_line);
	fill_cmde_name(cmd);
	return (SUCCESS);
}
