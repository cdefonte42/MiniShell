/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:03:20 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/05 15:12:58 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Retourne 1 si c est un metacharacter cad | < > ou backspace (8 \b) 
horizontal tab (9 \t) line feed (10 \n) vertical tab (11 \v) form feed (12 \f) 
ou carriage return (13 \r) */
int	ft_ismetachar(char c)
{
	if ((c >= 8 && c <= 13) || c == '|' || c == '<' || c == '>' || c == 32)
		return (1);
	return (0);
}

int	ft_isoperator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}
