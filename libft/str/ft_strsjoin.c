/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 14:49:15 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/22 11:23:15 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcat(char *s1, const char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = -1;
	while (s1[i] != '\0')
		i++;
	while (s2[++j] != '\0')
		s1[i + j] = s2[j];
	s1[i + j] = '\0';
	return (s1);
}

static char	*ft_strcpy(char *dst, const char *src)
{
	int	i;

	i = -1;
	while (src[++i])
		dst[i] = src[i];
	dst[i] = '\0';
	return (dst);
}

/**
 * @brief Allocates (with malloc(3)) and returns a new string,
 *  which is the result of the concatenation of ’s1’ and ’s3’
 * with 's2' in the middle.
 * @param s1 The prefix string.
 * @param s2 The middle string.
 * @param s3 The suffix string.
 * @return char* The new string.NULL if the allocation fails.
 */
char	*ft_strsjoin(char const *s1, char *s2, char const *s3)
{
	size_t	s1_size;
	size_t	s2_size;
	size_t	s3_size;
	char	*result;

	result = NULL;
	if (s1 && s2 && s3)
	{
		s1_size = ft_strlen(s1);
		s2_size = ft_strlen(s2);
		s3_size = ft_strlen(s2);
		result = malloc(sizeof(char) * (s1_size + s2_size + s3_size + 1));
		if (!result)
			return (NULL);
		ft_strcpy(result, s1);
		ft_strcat(result, s2);
		ft_strcat(result, s3);
	}
	return (result);
}
