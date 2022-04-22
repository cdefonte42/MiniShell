/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replacestr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 15:37:54 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/11 17:04:48 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_replacestr(char *dst, const char *t_replace, const char *replace_w)
{
	int		i;
	int		t_rep_len;
	int		len_final;
	char	*final;

	if (!dst || !replace_w || !t_replace)
		return (NULL);
	if (ft_strnstr(dst, t_replace, ft_strlen(dst)) == NULL)
		return (NULL);
	t_rep_len = ft_strlen(t_replace);
	len_final = (ft_strlen(replace_w) + (ft_strlen(dst) - t_rep_len)) + 1;
	final = ft_calloc(len_final, sizeof(char));
	if (!final)
		return (NULL);
	i = 0;
	while (ft_strncmp(dst + i, t_replace, t_rep_len) != 0)
		i++;
	ft_strlcpy(final, dst, i + 1);
	ft_strlcat(final, replace_w, len_final);
	ft_strlcat(final, dst + i + t_rep_len, len_final);
	return (final);
}
