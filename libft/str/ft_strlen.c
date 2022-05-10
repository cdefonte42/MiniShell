/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 11:26:38 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/08 16:59:14 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	n;

	n = 0;
	while (s && s[n] != '\0')
		n++;
	return (n);
}

size_t	ft_sli(const char *s)
{
	size_t	n;

	if (*s == '-' || *s == '+')
		s++;
	while (*s == '0')
		s++;
	n = 0;
	while (s[n] != '\0')
		n++;
	return (n);
}
