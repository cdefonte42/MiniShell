/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:30:54 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/19 11:18:27 by cdefonte         ###   ########.fr       */
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

int	search_all_heredoc(t_minishell *msh, t_cmde *cmd_lst)
{
	t_token			*tokens;

	if (g_status == -1)
		return (FAILURE);
	if (!cmd_lst || !cmd_lst->cmde_line)
		return (SUCCESS);
	tokens = cmd_lst->cmde_line;
	while (tokens)
	{
		if (tokens->type == heredoc)
		{
			if (rand_hdname(cmd_lst) == FAILURE)
				return (g_status = -1, FAILURE);
			g_status = heredoc_fork(msh, cmd_lst, &(tokens->next->str));
			if (g_status != 0)
				return (FAILURE);
		}
		tokens = tokens->next;
	}
	return (search_all_heredoc(msh, cmd_lst->next));
}

int	token_check_order(t_minishell *msh, t_cmde *cmd_lst)
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
		return (g_status = 2, FAILURE);
	}
	while (tokens)
	{
		if ((tokens->type >= op && prev_type >= op)
			|| (tokens->type >= op && !tokens->next && !cmd_lst->next))
		{
			ft_error("syntax error near unexpected token", tokens->str);
			return (g_status = 2, FAILURE);
		}
		prev_type = tokens->type;
		tokens = tokens->next;
	}
	return (token_check_order(msh, cmd_lst->next));
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
		return (SUCCESS);
	if (ft_check_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
	if (ft_fill_cmdelst(&(msh->cmde_lst), token_lst) == FAILURE)
	{
		g_status = -1;
		return (ft_tokenlst_free(token_lst), FAILURE);
	}
	if (token_check_order(msh, msh->cmde_lst) == FAILURE)
		return (FAILURE);
	if (search_all_heredoc(msh, msh->cmde_lst) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
