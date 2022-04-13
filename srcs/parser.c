/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/13 23:16:17 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

typedef struct s_token	t_cmde_line;

///* retourne le nombre de oken ayant pour type 'type' */
//int	ft_count_tokens_type(t_token *lst, t_token_type type)
//{
//	int	nbelem;
//
//	nbelem = 0;
//	while (lst)
//	{
//		if (lst->type == type)
//			nbelem++;
//		lst = lst->next;
//	}
//	return (nbelem);
//}
//
///* Transforme la liste cmde line en char **tab en ne prenant QUE ses words*/
//char	**ft_lst_to_char(t_token *lst)
//{
//	char	**argv;
//	int		i;
//	int		nbword;
//
//	i = 0;
//	nbword = ft_count_tokens_type(lst, word);
//	argv = malloc(sizeof(char *) * (nbword + 1));
//	if (!argv)
//		return (perror("malloc failed in ft_token_to_argv"), NULL);
//	while (lst && lst->type != spipe)
//	{
//		if (lst->type == word)
//		{
//			argv[i] = lst->str;
//			i++;
//			lst = lst->next;
//		}
//		else if (lst->type >= op)
//			lst = lst->next->next;
//	}
//	return (argv);
//}

/* Liste pour UNE commande, tous ses tokens. Cad tous les tokens de la liste 
token_lst jusqu'a l'operator control '|' COMPRIS (permet de realiser le pipe
avant de lancer l'exec de la commande). La token_lst doit etre propre*/
int	ft_get_cmdeline(t_token **token_lst, t_token **cmde_line)
{
	t_token	*token_toadd;
	int		reach_pipe;
	t_token	*tmp;

	reach_pipe = 0;
	while (token_lst && *token_lst && !reach_pipe)
	{
		token_toadd = ft_tokenlst_new((*token_lst)->str, (*token_lst)->type);
		if (!token_toadd)
			return (perror("malloc failed getcmdeline"), ft_tokenlst_free(cmde_line), FAILURE);
		ft_tokenlst_addback(cmde_line, token_toadd);
		if ((*token_lst)->type == spipe)
			reach_pipe = 1;
		tmp = *token_lst;
		*token_lst = (*token_lst)->next;
		free(tmp);
	}
	return (SUCCESS);
}

int	ft_get_cmdelinelst(t_list **lst, t_token **token_lst)
{
	t_token	*cmde_line;
	t_list	*new;
	void	(*del)(void *);
	
	del = &ft_tokenlst_free;
	while (token_lst && *token_lst)
	{
		cmde_line = NULL;
		if (ft_get_cmdeline(token_lst, &cmde_line) == FAILURE)
			return (perror("malloc failed getcmdelinelst"), ft_lstclear(lst, del), FAILURE);
		new = ft_lstnew(cmde_line);
		if (!new)
			return (perror("malloc failed getcmdelinelst"), ft_lstclear(lst, del), FAILURE);
		ft_lstadd_back(lst, new);

	}
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_token	*token_lst;
	t_list		*lst;
	t_list		*new;

	(void)ac;
	(void)av;
	token_lst = NULL;
	lst = NULL;
	new = NULL;
	if (!av[1])
		return (printf("Need 1 argument str pour tester debile\n"), 1);
	printf("Readline return equivqlent AV[1]=%s\n", av[1]);

	if (ft_tokener(&token_lst, av[1]) == FAILURE)
		return (1);
	if (!token_lst)	
		return (printf("Raw token list empty\n"), 0); //en gros line vide rien a faire
	if (ft_check_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(&token_lst), 1);

//	printf("___RAW TOKENs LISTE__\n");
//	for (t_token *head = token_lst; head != NULL; head = head->next)
//		printf("%s; type=%d\n", head->str, head->type);

	if (ft_get_cmdelinelst(&lst, &token_lst) == FAILURE)
	{
		printf("Failure de ft_get_cmdelinelst\n");
		ft_tokenlst_free(&token_lst);
		return (1);	
	}

	for (t_list *pouet = lst; pouet; pouet = pouet->next)
	{
		printf("___CMDE LINE___\n");
		for (t_token *line = (t_token *)(pouet->content); line; line = line->next)
			printf("%s; type=%d\n", line->str, line->type);
	}

	return (0);
}
