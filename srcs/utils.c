/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 10:03:20 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/10 18:26:55 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
/* _______ BONUS OPERATORS _________
	if (c1 == '&' && c2 == '&')
		return (1);
	if (c1 == '|' && c2 == '|')
		return (1);
*/
	return (0);
}

int	ft_iscontrol_operator(char c)
{
	if (c == '|')
		return (1);
/* _______ BONUS OPERATORS _________
	ATTENTION pb sur le '&' qui tout seul ne devrait pas et OK
	if (c == '&' || c == ')' || c == '(')
		return (1);
*/
	return (0);
}

/* Retourne 1 si c est une operateur */
int	ft_isoperator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
/* _______ BONUS OPERATORS _________
	ATTENTION pb sur le '&' qui tout seul ne devrait pas et OK
	if (c == '&' || c == ')' || c == '(')
		return (1);
*/
	return (0);
}

/* In the shell command language, a word consisting solely of underscores, 
digits, and alphabetics from the portable character set. 
The first character of a name is not a digit. */
int	ft_isname(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] >= '0' && str[0] <= '9')
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	ft_free_tabtab(char **tab)
{
	int		i;

	i = 0;
	while (tab && tab[i])
	{
		free(tab[i]);
		i++;	
	}
	free(tab);
}

int	ft_isblank(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

/* Compare c avec chaque caractere de set. Retourne 1 si c est parmis set. 0 si
c ne fait pas partis du set de caracteres. */
int	ft_isset(char c, char *set)
{
	int		i;

	i = 0;
	while (set && set[i])
	{
		if (c == set[i])
			return (1);
		i++;	
	}
	return (0);
}

char* toBinary(int n, int len)
{
    char* binary = (char*)malloc(sizeof(char) * len);
    int k = 0;
    for (unsigned i = (1 << (len - 1)); i > 0; i = i / 2) {
        binary[k++] = (n & i) ? '1' : '0';
    }
    binary[k] = '\0';
    return binary;
}
//
//int	main(void)
//{
//    int n = '|' & '|';
//    int len = 32;
//
//    char* binary = toBinary(n, len);
//    printf("The binary representation of %d is %s\n", n, binary);
//    free(binary);
//	return (0);
//}
