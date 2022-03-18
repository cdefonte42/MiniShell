/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 10:28:36 by mbraets           #+#    #+#             */
/*   Updated: 2022/03/18 10:36:11 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t needle_len; /* Length of NEEDLE.  */
	size_t haystack_len; /* Known minimum length of HAYSTACK.  */

	if (needle[0] == '\0')
		return (char *) haystack;

	/* Skip until we find the first matching char from NEEDLE.  */
	haystack = ft_strchr (haystack, needle[0]);
	if (haystack == NULL || needle[1] == '\0')
		return (char *) haystack;

	/* Ensure HAYSTACK length is at least as long as NEEDLE length.
		Since a match may occur early on in a huge HAYSTACK, use strnlen
		and read ahead a few cachelines for improved performance.  */
	needle_len = ft_strlen (needle);
	if (haystack_len < needle_len)
		return NULL;

	if (memcmp (haystack, needle, needle_len) == 0)
		return (char *) haystack;
}
