/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:30:54 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/05 10:21:30 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

extern int	g_status;

int	msh_isquoted(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '"')
			return (doubleq);
		else if (str[i] == '\'')
			return (singleq);
		i++;
	}
	return (nil);
}

int	token_check(t_minishell *msh, t_cmde *cmd_lst)
{
	t_token_type	prev_type;
	t_token			*tokens;

	prev_type = none;
	if (!cmd_lst || !cmd_lst->cmde_line)
		return (SUCCESS);
	tokens = cmd_lst->cmde_line;
	if (tokens->type == spipe)
	{
		ft_error("syntax error near unexpected token", tokens->str);
		g_status = 2;
		return (FAILURE);
	}
	while (tokens)
	{
		if ((tokens->type >= op && prev_type >= op)
			|| (tokens->type >= op && !tokens->next && !cmd_lst->next))
		{
			ft_error("syntax error near unexpected token", tokens->str);
			g_status = 2;
			return (FAILURE);
		}
		tokens = tokens->next;
	}
	return (token_check(msh, cmd_lst->next));
}

int	ft_parse(t_minishell *msh, char *line)
{
	t_token	*token_lst;

	token_lst = NULL;
	msh->cmde_lst = NULL;
	if (*line == 0)
		return (SUCCESS);
	if (ft_tokener(&token_lst, line) == FAILURE)
	{
		g_status = -1;
		return (FAILURE);
	}
	else if (!token_lst)
		return (printf("Raw token list empty\n"), SUCCESS);
	if (ft_check_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
	if (ft_fill_cmdelst(&(msh->cmde_lst), token_lst) == FAILURE)
	{
		g_status = -1;
		return (ft_tokenlst_free(token_lst), FAILURE);
	}
	if (token_check(msh, msh->cmde_lst) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

/*
//__________ DEBUG FCTS ________

void	ft_print_rawtokenlst(t_token *token_lst)
{
	printf("___RAW TOKENs LISTE__\n");
	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s; type=%d\n", head->str, head->type);
}

void	ft_print_cmdelst(t_cmde *cmde_lst)
{
	for (t_cmde *head = cmde_lst; head; head = head->next)
	{
		printf("___CMDE LINE ____\n");
		for (t_token *tokens = head->cmde_line; tokens; tokens = tokens->next)
			printf("%s\n", tokens->str);
		printf("___END CMDE LINE ____\n");
	}
}
*/
