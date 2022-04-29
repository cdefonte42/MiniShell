/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 18:02:37 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 18:04:54 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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

