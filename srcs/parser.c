/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/11 20:17:36 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"
#include <stdio.h>


//int ft_parse_line(t_minishell *msh); if == FAILURE exit minishell

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	t_token	*token_lst;
	t_cmde	*cmde_lst; //simple commands

	token_lst = NULL;
	if (!av[1])
		return (printf("Need 1 argument str pour tester debile\n"), 1);
	printf("Readline return equivqlent AV[1]=%s\n", av[1]);

	if (ft_tokener(&token_lst, av[1]) == FAILURE)
		return (1);
	if (!token_lst)
		return (printf("Raw token list empty\n"), 0); //en gros line vide rien a faire
	if (ft_check_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(&token_lst), 1);

// Expansion A FAIRE 
//	if (ft_expansion(&token_lst) == FAILURE)
//		return (ft_tokenlst_free(&token_lst), 1);

//	ft_fill_cmde_lst
	
	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s; type=%d\n", head->str, head->type);
	ft_tokenlst_free(&token_lst);
	return (0);
}
