/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/13 12:59:13 by cdefonte         ###   ########.fr       */
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
t_token	*ft_get_cmde_tokens(t_token *token_lst)
{
//	t_list	*lst;
	t_token	*cmde_line;
	t_token	*token_toadd;
	int		reach_pipe;

	cmde_line = NULL;
	reach_pipe = 0;
	while (token_lst && !reach_pipe)
	{
		token_toadd = ft_tokenlst_new(token_lst->str, token_lst->type);
		if (!token_toadd)
			return (NULL);
		ft_tokenlst_addback(&cmde_line, token_toadd);
		if (token_lst->type == spipe)
			reach_pipe = 1;
		token_lst = token_lst->next;
	}
	return (cmde_line);
/* Une fois la cmde_line extraite, mettre de cote les redir, chercher le name, faire les redirections, puis executer la cmde si name ressortis && redirs OK*/
}

int	main(int ac, char **av)
{
	t_token	*token_lst;
	t_cmde_line	*cmde_line;
	t_cmde		final_cmde;

	(void)ac;
	(void)av;
	token_lst = NULL;
	cmde_line = NULL;
	final_cmde = (t_cmde){0};
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

	printf("RAW TOKENs LISTE\n");
	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s; type=%d\n", head->str, head->type);

	printf("\n\n");
	printf("_____CMDE LINE 1______\n");
	/*_________ SIMPLE CMDE = cmde line= LIST ALL TOKENS UNTIL PIPE INCLUDED ____*/
	cmde_line = ft_get_cmde_tokens(token_lst);
	if (!cmde_line)
		return (1);
	for (t_cmde_line *pouet = cmde_line; pouet != NULL; pouet = pouet->next)
		printf("%s; type=%d\n", pouet->str, pouet->type);


	/*__________ Create char **argv ______*/
	/*_________ if PAS NAME => FORK __________*/
	/*___________if NAME && name = BIN => PAS FORK si PAS PIPE_____ */
	/*________ DO PIPE _______ */
	/*_________ DO REDIRS  ____________*/
	char	**argv;
	argv = ft_lst_to_char(cmde_line);
	if (!argv)
		return (perror("PAS ARGV"), 1);
	for (int i = 0; argv[i]; i++)
		printf("argv[%d]=%s\n", i, argv[i]);

	final_cmde.cmde_line = cmde_line;
	final_cmde.argv = argv;
	if (argv[0] && ft_isbin(argv[0]) && !final_cmde.piped)
	{
		//ft_execbin(final_cmde);
		printf("EXEC BIN\n");
	}

	ft_tokenlst_free(&token_lst);
	return (0);
}
