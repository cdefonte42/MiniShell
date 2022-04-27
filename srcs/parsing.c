/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 18:30:54 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/27 18:25:46 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
//#include "varenv.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

/* Liste pour UNE commande, tous ses tokens. Cad tous les tokens de la liste 
token_lst jusqu'a l'operator control '|' COMPRIS (permet de realiser le pipe
avant de lancer l'exec de la commande). La token_lst doit etre propre*/
t_token	*ft_extract_cmdeline(t_token **token_lst)
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
			tmp = *token_lst;
			*token_lst = (*token_lst)->next;
			tmp->next = NULL;
		}
		else
			*token_lst = (*token_lst)->next;
	}
	return (cmde_line);
}

int	ft_fill_cmdelst(t_cmde **alst, t_token *token_lst)
{
	t_cmde	*new_cmde;
	t_token	*cmde_line;

	while (token_lst)
	{
		cmde_line = ft_extract_cmdeline(&token_lst);
		if (cmde_line)
		{
			new_cmde = ft_cmdelst_new(cmde_line);
			if (!new_cmde)
				return (ft_cmdelst_clear(*alst), FAILURE);
			ft_cmdelst_addback(alst, new_cmde);
		}
	}
	return (SUCCESS);
}

void test(int q)
{
	
	printf("%d\n", q);
}

int	msh_isquoted(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '"')
			return (doubleq);
		else if (str[i] == '\'')
			return (singleq);
		i++;
	}
	return (nil);
}

int	ft_heredoc(char *delimiter, int *fdin)
{
	char	*tmp_name;
	char	*line;
	t_quote_type	quoted;

	quoted = msh_isquoted(delimiter);
	if (remove_quote(&delimiter) == FAILURE)
		return (perror("ft_heredoc remove auote failed"), FAILURE);
	signal(SIGINT, test);
	tmp_name = "./tmpfiletest";
	if (*fdin != 0 && close(*fdin) == -1)
		return (perror("ft_heredoc closing fd failed"), FAILURE);
	*fdin = open(tmp_name, O_CREAT | O_WRONLY | O_TRUNC, 00644);
	if (*fdin == -1)
		return (perror("ft_heredoc opening fd failed"), FAILURE);
	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line)
	{
		if (ft_strlen(line) > ft_strlen(delimiter) && !ft_strncmp(line, delimiter, ft_strlen(line) - 1))
			break ;
//		if (quoted != nil)
//			ft_expand_str(());
		ft_putstr_fd(line, *fdin);
		free(line);
		line = NULL;
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (line == NULL)
			ft_putstr_fd("heredoc line null\n", 2);
	}
	free(line);
	line = NULL;
	close(*fdin);
	*fdin = open(tmp_name, O_RDONLY);
	return (SUCCESS);
}

int	ft_lala(t_cmde *cmd_lst)
{
	t_token_type	prev_type;
	t_token			*tokens;

	prev_type = none;
	if (!cmd_lst || !cmd_lst->cmde_line)
		return (SUCCESS);
	tokens = cmd_lst->cmde_line;
	if (tokens->type == spipe)
	{
		ft_error("syntax error near unexpected token", tokens->str);
		return (FAILURE);
	}
	while (tokens)
	{
		if ((tokens->type >= op && prev_type >= op)
			|| (tokens->type >= op && !tokens->next && !cmd_lst->next))
		{
			ft_error("syntax 2error near unexpected token", tokens->str);
			return (FAILURE);
		}
		else if (tokens->type == heredoc)
		{
			//if (heredoc_fork())
			if (ft_heredoc(tokens->next->str, &(cmd_lst->pipefd[r_end])) == FAILURE)
				return (FAILURE);
		}
		tokens = tokens->next;
	}
	return (ft_lala(cmd_lst->next));
}

int	ft_parse(t_minishell *msh, char *line)
{
	t_token	*token_lst;

	token_lst = NULL;
	msh->cmde_lst = NULL;
	if (*line == 0)
	{
		msh->cmde_lst = NULL;
		return (SUCCESS);
	}
	if (ft_tokener(&token_lst, line) == FAILURE)
		return (FAILURE);
	else if (!token_lst)
		return (printf("Raw token list empty\n"), SUCCESS);
	if (ft_check_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
	if (ft_fill_cmdelst(&(msh->cmde_lst), token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
	if (ft_lala(msh->cmde_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), FAILURE);
	
// parcourir les tokens de la cmde (lst de tokens). Des aue tombe sur here doc fait.
// des aue tombe sur unexpected, cad first token is pipe, ou after token->type >= op
// on a un autreop, error unexpected token close les bordels et ciao. Attention a
// faire catch le signal de ctrl + D et ctr+ C.
//	if (ft_operator_order(lst) == FAILURE)
//		return (ft_tokenlst_free(token_lst), ft_cmdelst_clear(msh->cmde_lst), FAILURE);
	return (SUCCESS);
}

/*__________ DEBUG FCTS ________*/

void	ft_print_rawtokenlst(t_token *token_lst)
{
	printf("___RAW TOKENs LISTE__\n");
	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s; type=%d\n", head->str, head->type);
}

void	ft_print_cmdelst(t_cmde *cmde_lst)
{
	for (t_cmde *head = cmde_lst; head; head = head->next)
	{
		printf("___CMDE LINE ____\n");
		for (t_token *tokens = head->cmde_line; tokens; tokens = tokens->next)
			printf("%s\n", tokens->str);
		printf("___END CMDE LINE ____\n");
	}
}
