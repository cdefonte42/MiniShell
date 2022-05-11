/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmde_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 14:28:13 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/11 16:46:23 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
#include "minishell.h"
#include "libft.h"

extern int	g_status;

void	fill_cmde_name(t_cmde *cmd)
{
	t_cmde	*lst;
	t_token	*cmd_tokens;

	lst = cmd;
	cmd_tokens = lst->cmde_line;
	while (cmd_tokens && lst->name_token == NULL)
	{
		if (cmd_tokens->type >= op && cmd_tokens->next)
			cmd_tokens = cmd_tokens->next->next;
		else
			lst->name_token = cmd_tokens;
	}
}

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
	t_cmde	*lst;

	while (token_lst)
	{
		cmde_line = ft_extract_cmdeline(&token_lst);
		if (cmde_line)
		{
			new_cmde = ft_cmdelst_new(cmde_line);
			if (!new_cmde)
				return (ft_cmdelst_clear(alst), FAILURE);
			ft_cmdelst_addback(alst, new_cmde);
		}
	}
	lst = *alst;
	while (lst)
	{
		fill_cmde_name(lst);
		lst = lst->next;
	}
	return (SUCCESS);
}
