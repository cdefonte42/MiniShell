/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 11:13:03 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/21 19:14:39 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_strlchr(char c, const char *set)
{
	int	i;

	i = -1;
	while (set[++i] != '\0')
		if (set[i] == c)
			return (1);
	return (0);
}
/**
 * strtrim strips all second argument characters from the beginning and end of the first string.
 * @param s1 The string to strips
de la liste.
 * @param set List of character
à la liste.
 * @return New character
 */
// strtrim removes all whitespace characters from the beginning and the end of a string. As whitespace is counted everything for which isspace(3) returns true. 
char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*result;

	result = NULL;
	if (s1 && set)
	{
		start = 0;
		end = ft_strlen(s1);
		while (s1[start] && ft_strlchr(s1[start], set))
			start++;
		while (end > start && ft_strlchr(s1[end - 1], set))
			end--;
		result = malloc(sizeof(char) * (end - start + 1));
		if (!result)
			return (NULL);
		ft_strlcpy(result, s1 + start, end - start + 1);
	}
	return (result);
}
