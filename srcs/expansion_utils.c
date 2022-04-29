/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 18:02:37 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 18:57:03 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "export.h"

extern int	g_status;

int	get_start(t_token *token, int start, int *qtype)
{
	while (token->str[start] && (*qtype == singleq || token->str[start] != '$'))
	{
		if (token->str[start] == '"' && (*qtype != singleq || *qtype == 0))
			*qtype = *qtype ^ doubleq;
		else if (token->str[start] == '\'' && (*qtype != doubleq || *qtype == 0))
			*qtype = *qtype ^ singleq;
		start++;
	}
	return (start);
}

/* Remplace dans src len characteres, a partir dee l'index start, par la chaine
rep_w. Retourne FAILURE en cas de malloc error. Si src = $prout er rep_w = NULL,
le resulatat donne une str[0] = 0; */
int	exp_replacewith(char **src, int start, int len, char *rep_w)
{
	int		len_final;
	char	*final;

	if (!*src || len == 0)
		return (NULL);
	len_final = ft_strlen(*src) - len + ft_strlen(rep_w);
	final = ft_calloc(len_final + 1, sizeof(char));
	if (!final)
		return (FAILURE);
	ft_strlcpy(final, *src, start + 1);
	if (rep_w)
		ft_strlcat(final, rep_w, len_final + 1);
	ft_strlcat(final, *src + start + len, len_final + 1);
	free(*src);
	*src = final;
	return (SUCCESS);
}

