/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 20:00:55 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 17:29:09 by cdefonte         ###   ########.fr       */
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

void	ft_tokenlst_pop(t_token **lst, int i)
{
	t_token	*prev;
	t_token	*to_rm;
	int		j;

	j = 0;
	to_rm = *lst;
	prev = NULL;
	while (to_rm && j < i)
	{
		prev = to_rm;
		to_rm = to_rm->next;
		j++;
	}
	if (to_rm == NULL)
		return ;
	if (prev == NULL)
		*lst = to_rm->next;
	else
		prev->next = to_rm->next;
	free(to_rm->str);
	to_rm->str = NULL;
	free(to_rm);
	to_rm = NULL;
}
