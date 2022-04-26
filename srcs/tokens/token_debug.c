/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 12:20:18 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/26 12:23:10 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "stdio.h"

void	ft_print_tokenlst(t_token *lst)
{
	while (lst)
	{
		printf("__%s__\n", lst->str);
		lst = lst->next;
	}
}
