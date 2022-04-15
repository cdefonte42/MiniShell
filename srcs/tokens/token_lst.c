/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:00:55 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/15 12:32:18 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include <stdlib.h>

t_token	*ft_tokenlst_new(char *str, int type)
{
	t_token	*new_token;
	
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = str;
	new_token->type = type;
	new_token->next = NULL;	
	return (new_token);
}

t_token	*ft_tokenlst_last(t_token *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);	
}

void	ft_tokenlst_addback(t_token **alst, t_token *new)
{
	t_token	*last;

	if (alst == NULL || new == NULL)
		return ;
	if (*alst == NULL)
	{
		*alst = new;
		return ;
	}
	last = ft_tokenlst_last(*alst);
	last->next = new;
}

void	ft_tokenlst_free(t_token *lst)
{
	t_token	*prev;

	while (lst)
	{
		prev = lst;
		lst = lst->next;
		free(prev->str);
		prev->str = NULL;
		free(prev);
		prev = NULL;
	}
}

int	ft_tokenlst_iteri(t_token *lst, int (*f)(char *))
{
	while (lst)
	{
		if ((*f)(lst->str) == FAILURE)
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

