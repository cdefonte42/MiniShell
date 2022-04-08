/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_until.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:21:03 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/08 17:36:31 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Alloue et retourne une chaine, copie de src juqu'au charactere 'stop' */
char	*ft_strdup_until(const char *src, char stop)
{
	size_t	size;
	size_t	i;
	char	*temp;
	char	*res;

	size = 0;
	temp = (char *)src;
	while (temp && temp[size] && temp[size] != stop)
		size++;
	res = malloc(sizeof(char) * (size * 1));
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
