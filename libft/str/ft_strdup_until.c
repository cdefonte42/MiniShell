/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_until.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:21:03 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/21 14:43:17 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Alloue et retourne une chaine, copie de src juqu'à i < stop */
char	*ft_strdup_until_i(const char *src, size_t stop)
{
	size_t	size;
	size_t	i;
	char	*res;

	if (!src)
		return (NULL);
	size = ft_strlen(src);
	res = malloc(sizeof(char) * (stop + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < stop && i < size)
	{
		res[i] = src[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

/* Alloue et retourne une chaine, copie de src juqu'au charactere 'stop' */
char	*ft_strdup_until_c(const char *src, char stop)
{
	size_t	size;
	size_t	i;
	char	*temp;
	char	*res;

	if (!src)
		return (NULL);
	size = 0;
	temp = (char *)src;
	while (temp && temp[size] && temp[size] != stop)
		size++;
	res = malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i] = temp[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
