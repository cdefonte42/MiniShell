/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:30:54 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 16:12:22 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
//#include "varenv.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

extern int	g_status;

/* Liste pour UNE commande, tous ses tokens. Cad tous les tokens de la liste 
token_lst jusqu'a l'operator control '|' COMPRIS (permet de realiser le pipe
avant de lancer l'exec de la commande). La token_lst doit etre propre*/
t_token	*ft_extract_cmdeline(t_token **token_lst)
{
	t_token	*cmde_line;
	int		reach_pipe;
	t_token	*tmp;

	reach_pipe = 0;
	cmde_line = *token_lst;
	while (token_lst && *token_lst && !reach_pipe)
	{
		if ((*token_lst)->type == spipe)
		{
			reach_pipe = 1;
			tmp = *token_lst;
			*token_lst = (*token_lst)->next;
			tmp->next = NULL;
		}
		else
			*token_lst = (*token_lst)->next;
	}
	return (cmde_line);
}

int	ft_fill_cmdelst(t_cmde **alst, t_token *token_lst)
{
	t_cmde	*new_cmde;
	t_token	*cmde_line;

	while (token_lst)
	{
		cmde_line = ft_extract_cmdeline(&token_lst);
		if (cmde_line)
		{
			new_cmde = ft_cmdelst_new(cmde_line);
			if (!new_cmde)
				return (ft_cmdelst_clear(*alst), FAILURE);
			ft_cmdelst_addback(alst, new_cmde);
		}
	}
	return (SUCCESS);
}

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

int	token_check(t_minishell *msh, t_cmde *cmd_lst) // rename from lala tmp name
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
		return (SUCCESS);
	}
	while (tokens)
	{
		if ((tokens->type >= op && prev_type >= op)
			|| (tokens->type >= op && !tokens->next && !cmd_lst->next))
		{
			ft_error("syntax error near unexpected token", tokens->str);
			g_status = 2;
			return (SUCCESS);
		}
		else if (tokens->type == heredoc)
		{
			if (rand_hdname(cmd_lst) == FAILURE)
				return (FAILURE);
			if (heredoc_fork(msh, cmd_lst, tokens->next->str) == FAILURE)
			{
				perror("fork heredoc failed should quit msh");
				return (FAILURE);
			}
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
	{
		msh->cmde_lst = NULL;
		return (SUCCESS);
	}
	if (ft_tokener(&token_lst, line) == FAILURE)
		return (FAILURE);
	else if (!token_lst)
		return (printf("Raw token list empty\n"), SUCCESS);
	if (ft_check_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
	if (ft_fill_cmdelst(&(msh->cmde_lst), token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
	if (token_check(msh, msh->cmde_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
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
