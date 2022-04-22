/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_checks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:42:30 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/22 18:58:19 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
int	remove_quote(t_token *lst);
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

char	*ft_replacestr_i(int i, char *dst, const char *t_replace, const char *replace_w)
{
	int		t_rep_len;
	int		len_final;
	char	*final;

	if (!dst || !t_replace)
		return (NULL);
	if (ft_strnstr(dst, t_replace, ft_strlen(dst)) == NULL)
		return (NULL);
	t_rep_len = ft_strlen(t_replace);
	len_final = (ft_strlen(replace_w) + (ft_strlen(dst) - t_rep_len)) + 1;
	final = ft_calloc(len_final, sizeof(char));
	if (!final)
		return (NULL);
	i = 0;
	while (ft_strncmp(dst + i, t_replace, t_rep_len) != 0)
		i++;
	ft_strlcpy(final, dst, i + 1);
	if (replace_w)
		ft_strlcat(final, replace_w, len_final);
	ft_strlcat(final, dst + i + t_rep_len, len_final);
	return (final);
}

int	remove_quote(t_token *lst)
{
	int				i;
	t_quote_type	inquote;
	char			*newstr;
	char			*str;
	
	i = 0;
	inquote = nil;
	str = lst->str;
	while (str && str[i])
	{
		if (str[i] == '"' && inquote != singleq)
		{
			inquote = inquote ^ doubleq;
			newstr = ft_replacestr_i(i, str, "\"", NULL);
			if (!newstr)
				return (perror("remove_quote"), FAILURE);
			free(str);
			str = newstr;
		}
		else if (str[i] == '\'' && inquote != doubleq)
		{
			inquote = inquote ^ singleq;
			newstr = ft_replacestr_i(i, str, "\'", NULL);
			if (!newstr)
				return (perror("remove_quote"), FAILURE);
			free(str);
			str = newstr;
		}
		else
			i++;
	}
	lst->str = str;
	return (SUCCESS);
}
