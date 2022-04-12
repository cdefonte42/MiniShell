/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:42:30 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/12 12:40:01 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "libft.h"

int	ft_quotes_check(char *str)
{
	int				i;
	t_quote_type	inquote;

	i = 0;
	inquote = 0;
	while (str && str[i])
	{
		if (str[i] == '"' && (inquote != singleq || inquote == 0))
			inquote = inquote ^ doubleq;
		if (str[i] == '\'' && (inquote != doubleq || inquote == 0))
			inquote = inquote ^ singleq;
		i++;
	}
	if (inquote)
		return (ft_putstr_fd("Error: quoted field never ends\n", 2), FAILURE);
	return (SUCCESS);
}

void	ft_set_operator_type(t_token *elem)
{
	if (elem->type != op)
		return ;
	if (ft_strcmp(elem->str, ">>") == 0)
		elem->type = redirapp;
	else if (ft_strcmp(elem->str, ">>") == 0)
		elem->type = redirapp;
	else if (ft_strcmp(elem->str, "<<") == 0)
		elem->type = heredoc;
	else if (ft_strcmp(elem->str, "<") == 0)
		elem->type = redirin;
	else if (ft_strcmp(elem->str, ">") == 0)
		elem->type = redirout;
	else if (ft_strcmp(elem->str, "|") == 0)
		elem->type = spipe;
}

/* Verifie que'il n'y ai pas 2 operateurs consecutifs. */
int	ft_operator_order(t_token *lst)
{
	t_token_type	prev_type;

	prev_type = none;
	while (lst)
	{
		if ((lst->type >= op && prev_type >= op && prev_type != spipe) 
			|| (lst->next == NULL && lst->type == op)
			|| (lst->type == spipe && prev_type == spipe))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(lst->str, 2);
			ft_putstr_fd("'\n", 2);
			return (FAILURE);
		}
		prev_type = lst->type;
		lst = lst->next;
	}
	return (SUCCESS);
}

int	ft_check_tokens(t_token *lst)
{
	t_token	*tmp;

	tmp = lst;
	if (ft_tokenlst_iteri(lst, &ft_quotes_check) == FAILURE)
		return (FAILURE);
	while (tmp)
	{
		ft_set_operator_type(tmp);
		tmp = tmp->next;
	}
	if (ft_operator_order(lst) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
