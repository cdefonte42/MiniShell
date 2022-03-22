/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 12:40:13 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/22 11:35:46 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Applies the function ’f’ on each character of
the string passed as argument, passing its index
as first argument. Each character is passed by
address to ’f’ to be modified if necessary.
 * 
 * @param s The string on which to iterate.
 * @param f The function to apply to each character.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;
	size_t	len;

	i = 0;
	if (s != NULL && f)
	{
		len = ft_strlen(s);
		while (i < len)
		{
			(*f)(i, s);
			i++;
			s++;
		}
	}
}
