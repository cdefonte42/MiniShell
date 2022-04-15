/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/15 11:33:39 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

typedef struct s_token	t_cmde_line;

/* retourne le nombre de oken ayant pour type 'type' */
int	ft_count_tokens_type(t_token *lst, t_token_type type)
{
	int	nbelem;

	nbelem = 0;
	while (lst)
	{
		if (lst->type == type)
			nbelem++;
		else if (type == word && lst->type >= op && lst->next)
			lst = lst->next;
		lst = lst->next;
	}
	return (nbelem);
}

/* Transforme la liste cmde line en char **tab en ne prenant QUE ses words*/
char	**ft_lst_to_char(t_token *lst)
{
	char	**argv;
	int		i;
	int		nbword;


	i = 0;
	nbword = ft_count_tokens_type(lst, word);
	argv = malloc(sizeof(char *) * (nbword + 1));
	if (!argv)
		return (perror("malloc failed in ft_token_to_argv"), NULL);
	while (lst && lst->type != spipe)
	{
		if (lst->type == word)
		{
			argv[i] = lst->str;
			i++;
			lst = lst->next;
		}
		else if (lst->type >= op)
			lst = lst->next->next;
	}
	return (argv);
}

/* Liste pour UNE commande, tous ses tokens. Cad tous les tokens de la liste 
token_lst jusqu'a l'operator control '|' COMPRIS (permet de realiser le pipe
avant de lancer l'exec de la commande). La token_lst doit etre propre*/
t_token	*ft_split_tokens(t_token **token_lst)
{
	t_token	*cmde_line;
	int		reach_pipe;
	t_token	*tmp;

	reach_pipe = 0;
	cmde_line = *token_lst;
	while (token_lst && *token_lst && !reach_pipe)
	{
		if ((*token_lst)->type == spipe)
		{
			reach_pipe = 1;
		// FAIRE PIPE ICI ////
			tmp = *token_lst;
			*token_lst = (*token_lst)->next;
			tmp->next = NULL;
		}
		else
			*token_lst = (*token_lst)->next;
	}
	return (cmde_line);
}

int	ft_fill_cmdelst(t_list **alst, t_token *token_lst)
{
	t_cmde	*new_cmde;
	t_list	*new_elem;
	
	new_cmde = malloc(sizeof(t_cmde));
	if (!new_cmde)
		return (FAILURE);
	while (token_lst)
	{
		new_cmde->cmde_line = ft_split_tokens(&token_lst);
		if (new_cmde->cmde_line != NULL)
		{
			new_elem = ft_lstnew(new_cmde);
			if (!new_elem)
				return (FAILURE);
			ft_lstadd_back(alst, new_elem);
		}
		else
			free(new_cmde);
	}
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	t_token	*token_lst;
	t_list	*cmde_lst;

	(void)ac;
	(void)av;
	token_lst = NULL;
	cmde_lst = NULL;
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

	if (ft_fill_cmdelst(&cmde_lst, token_lst) == FAILURE)
		return (ft_tokenlst_free(&token_lst), 1);

	for (t_list *headlst = cmde_lst; headlst; headlst = headlst->next)
	{
		for (t_cmde *cmde = (t_cmde *)headlst->content; cmde; cmde = cmde->next)
				printf("%s\n", cmde->cmde_line->str);
	}


	return (0);
}
