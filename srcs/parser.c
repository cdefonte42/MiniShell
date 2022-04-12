/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/12 13:05:45 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "libft.h"
#include <stdio.h>

typedef struct s_token	t_cmde_line;

/* Liste pour UNE commande, tous ses tokens. Cad tous les tokens de la liste 
token_lst jusqu'a l'operator control '|'. La token_lst doit etre propre*/
t_token	*ft_get_cmde_tokens(t_token *token_lst)
{
//	t_list	*lst;
	t_token	*cmde_line;
	t_token	*token_toadd;

	cmde_line = NULL;
	while (token_lst && token_lst->type != spipe)
	{
		if (token_lst->type != spipe)
		{
			token_toadd = ft_tokenlst_new(token_lst->str, token_lst->type);
			if (!token_toadd)
				return (NULL);
			ft_tokenlst_addback(&cmde_line, token_toadd);
		}
		token_lst = token_lst->next;
	}
	return (cmde_line);
/* Une fois la cmde_line extraite, mettre de cote les redir, chercher le name, faire les redirections, puis executer la cmde si name ressortis && redirs OK*/
}

int	main(int ac, char **av)
{
	t_token	*token_lst;
	t_cmde_line	*cmde;

	(void)ac;
	(void)av;
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

// Expansion A FAIRE ET SPLITING WORDS genre ZA="echo prout" donne 1 token lors cut mais lors expansion pus splitting donne deux tokens !!!
//	if (ft_expansion(&token_lst) == FAILURE) 
//		return (ft_tokenlst_free(&token_lst), 1);

//	ft_fill_cmde_lst
	
	printf("RAW TOKENs LISTE\n");
	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s; type=%d\n", head->str, head->type);

	printf("\n\n");
	printf("_____CMDE LINE 1______\n");
	cmde = ft_get_cmde_tokens(token_lst);
	if (!cmde)
		return (1);
	for (t_cmde_line *pouet = cmde; pouet != NULL; pouet = pouet->next)
		printf("%s; type=%d\n", pouet->str, pouet->type);


	ft_tokenlst_free(&token_lst);
	return (0);
}
