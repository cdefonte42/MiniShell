/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 15:01:18 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/30 21:48:14 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"

int	ft_iscontrol_operator(char c)
{
	if (c == '|')
		return (1);
	return (0);
}

/* Retourne 1 si c est une operateur */
int	ft_isoperator(char c)
{
	if (c == '|')
		return (1);
	else if (c == '<')
		return (1);
	else if (c == '>')
		return (1);
	return (0);
}

/* Retourne 1 si c est un metacharacter cad | < > ou backspace (8 \b) 
horizontal tab (9 \t) line feed (10 \n) vertical tab (11 \v) form feed (12 \f) 
ou carriage return (13 \r) ou space (32)*/
int	ft_ismetachar(char c)
{
	if ((c >= 8 && c <= 13) || c == '|' || c == '<' || c == '>' || c == ' ')
		return (1);
	return (0);
}

/* Retourne 1 si c1 + c2 forment un operateur. ATTENTION c1 DOIT etre 
un operator.*/
int	ft_formoperator(char c1, char c2)
{
	if (c1 == '>' && c2 == '>')
		return (1);
	if (c1 == '<' && c2 == '<')
		return (1);
	return (0);
}
