/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_replacestr_i.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 10:21:48 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 16:57:14 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_replacestri(int i, char *dst, const char *t_rep, const char *rep_w)
{
	int		t_rep_len;
	int		len_final;
	char	*final;

	if (!dst || !t_rep)
		return (NULL);
	if (ft_strnstr(dst, t_rep, ft_strlen(dst)) == NULL)
		return (NULL);
	t_rep_len = ft_strlen(t_rep);
	len_final = (ft_strlen(rep_w) + (ft_strlen(dst) - t_rep_len)) + 1;
	final = ft_calloc(len_final, sizeof(char));
	if (!final)
		return (NULL);
	ft_strlcpy(final, dst, i + 1);
	if (rep_w)
		ft_strlcat(final, rep_w, len_final);
	ft_strlcat(final, dst + i + t_rep_len, len_final);
	return (final);
}
