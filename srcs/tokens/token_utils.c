/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:00:55 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/16 12:42:34 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <stdlib.h>

int	ft_tokenlst_iteri(t_token *lst, int (*f)(t_token *))
{
	while (lst)
	{
		if ((*f)(lst) == FAILURE)
			return (FAILURE);
		lst = lst->next;
	}
	return (SUCCESS);
}

int	ft_tokenlst_iteri_s(t_token *lst, int (*f)(char **))
{
	while (lst)
	{
		if ((*f)(&(lst->str)) == FAILURE)
			return (FAILURE);
		lst = lst->next;
	}
	return (SUCCESS);
}

int	ft_tokenlst_size(t_token *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}

/*Remove de la token_lst les elements dont la str est NULL ou str[0] == NULL */
void	ft_remove_empty_token(t_token **token_lst)
{
	t_token	*head;
	t_token	*prev;

	head = *token_lst;
	prev = NULL;
	while (head)
	{
		if ((head->str == NULL || head->str[0] == 0) && prev && prev->type != heredoc)
		{
			ft_tokenlst_pop(token_lst, head, prev);
			head = prev->next;
		}
		else
		{
			prev = head;
			head = head->next;
		}
	}
}

/* Rajoute l'element 'new' apres l'elemen 'elem'. ATTENTION elem doit etre non
NULL */
void	ft_tokenlst_addafter(t_token *elem, t_token *new)
{
	t_token	*next;

	next = elem->next;
	elem->next = new;
	new->next = next;
}
