/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:42:30 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/04 12:32:28 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokens.h"
#include "libft.h"
#include <stdio.h>

int	ft_quotes_check(t_token *lst)
{
	int				i;
	t_quote_type	inquote;
	char			*str;

	i = 0;
	inquote = nil;
	str = lst->str;
	while (str && str[i])
	{
		if (str[i] == '"' && (inquote != singleq || inquote == nil))
			inquote = inquote ^ doubleq;
		if (str[i] == '\'' && (inquote != doubleq || inquote == nil))
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

int	remove_quote(char **str)
{
	int		i;
	int		inquote;
	char	*newstr;

	i = 0;
	inquote = nil;
	while (str && *str && (*str)[i])
	{
		set_curr_quote((*str)[i], &inquote);
		if (((*str)[i] == '"' && inquote != singleq)
		|| ((*str)[i] == '\'' && inquote != doubleq))
		{
			if ((*str)[i] == '"')
				newstr = ft_replacestri(i, *str, "\"", NULL);
			else if ((*str)[i] == '\'')
				newstr = ft_replacestri(i, *str, "\'", NULL);
			if (!newstr)
				return (perror("remove_quote"), FAILURE);
			free(*str);
			*str = newstr;
		}
		else
			i++;
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
	return (SUCCESS);
}
