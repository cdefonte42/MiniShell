/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 12:00:49 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/22 11:36:31 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Applies the function ’f’ to each character of the
string ’s’, and passing its index as first argument
to create a new string (with malloc(3)) resulting
from successive applications of ’f’.
 * 
 * @param s The string on which to iterate.
 * @param f The function to apply to each character.
 * @return char* The string created from the successive applications
of ’f’.
Returns NULL if the allocation fails.
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	i;
	char	*result;

	result = NULL;
	if (s && f)
	{
		result = (char *)ft_calloc(ft_strlen(s) + 1, sizeof(char));
		if (!result)
			return (NULL);
		i = 0;
		while (i < ft_strlen(s))
		{
			result[i] = (*f)(i, s[i]);
			i++;
		}
		result[i] = '\0';
	}
	return (result);
}
