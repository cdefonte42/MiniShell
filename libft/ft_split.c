/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 12:23:39 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/22 11:33:45 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countword(char const *s, char c)
{
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	if (!*s)
		return (0);
	while (s[i] && s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		if (s[i] == c)
		{
			len++;
			while (s[i] && s[i] == c)
				i++;
			continue ;
		}
		i++;
	}
	if (s[i - 1] != c)
		len++;
	return (len);
}

static char	**ft_freetab(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static char	**ft_allocword(char const *s, char c)
{
	size_t	len;
	int		i;
	int		j;
	char	**strs;

	len = 0;
	i = 0;
	strs = malloc(sizeof(char *) * (ft_countword(s, c) + 1));
	if (!strs)
		return (NULL);
	while (s[i] != '\0')
	{
		if (s[i] && s[i] == c)
			i++;
		j = 0;
		while (s[i] && s[i++] != c)
			j++;
		if (j)
		{
			strs[len] = malloc((j + 1) * sizeof(char));
			if (!strs[len++])
				return (ft_freetab(strs));
		}
	}
	return (strs);
}

/**
 * @brief Allocates (with malloc(3)) and returns an array
of strings obtained by splitting ’s’ using the
character ’c’ as a delimiter. The array must end
with a NULL pointer.
 * 
 * @param s The string to be split.
 * @param c The delimiter character
 * @return char** The array of new strings resulting from the split.
NULL if the allocation fails.
 */
char	**ft_split(char const *s, char c)
{
	char	**strs;
	size_t	len;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	strs = ft_allocword(s, c);
	if (strs == NULL)
		return (NULL);
	i = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] && s[i] == c)
			i++;
		j = 0;
		while (s[i] && s[i] != c)
			strs[len][j++] = s[i++];
		if (j)
			strs[len++][j] = '\0';
	}
	strs[len] = NULL;
	return (strs);
}
