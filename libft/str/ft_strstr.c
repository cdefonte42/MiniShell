/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 10:28:36 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/29 17:00:18 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	needle_len;
	size_t	haystack_len;

	if (needle[0] == '\0')
		return ((char *)haystack);
	haystack = ft_strchr(haystack, needle[0]);
	if (haystack == NULL || needle[1] == '\0')
		return ((char *) haystack);
	haystack_len = ft_strlen(haystack);
	needle_len = ft_strlen(needle);
	if (haystack_len < needle_len)
		return (NULL);
	if (ft_memcmp(haystack, needle, needle_len) == 0)
		return ((char *) haystack);
}
