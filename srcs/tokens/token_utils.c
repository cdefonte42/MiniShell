/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:00:55 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/27 12:48:12 by cdefonte         ###   ########.fr       */
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


int	quote_ornot(t_token *lst, int (*f)(char **))
{
	while (lst)
	{
		if (lst->qtype != nil && (*f)(&(lst->str)) == FAILURE)
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
