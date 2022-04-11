/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 11:51:24 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/11 15:51:18 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"

/* Set la valeur du type de token en fonction du caractere c SSI le type actuel
est 'none' (default). */
void	ft_char_type(t_token_type *type, char c)
{
	if (ft_isoperator(c))
		*type = op;
	else if (ft_isblank(c))
		*type = blank;
//	else if (c == '\'')
//		*type = squoted;
//	else if (c == '"')
//		*type = dquoted;
	else
		*type = word;
}

/* Retourne la taille du token en fonction de son type */
int	ft_delimit_token(char *line, t_token_type *token_type)
{
	int				len;
	t_quote_type	inquote;

	len = 0;
	inquote = 0;
	while (*token_type != op && line && line[len])
	{
		if (line[len] == '"' && (inquote != singleq || inquote == 0))
			inquote = inquote ^ doubleq;
		if (line[len] == '\'' && (inquote != doubleq || inquote == 0))
			inquote = inquote ^ singleq;
		if (ft_ismetachar(line[len]) && !inquote)
			return (len);
		len++;
	}
	if (*token_type == op)
	{
		len = 1;
		if (ft_isoperator(line[0])
			&& ft_formoperator(line[0], line [1]))
			len = 2;
	}
	return (len);
}

t_token	*ft_create_token(char *line, int start, int len, int type)
{
	t_token	*new_token;

	new_token = ft_tokenlst_new(NULL, type);
	if (!new_token)
		return (perror("token_delimiter malloc failed"), NULL);
	new_token->str = ft_substr(line, start, len);
	if (!new_token->str)
	{
		ft_tokenlst_free(&new_token);
		return (perror("token_delimiter malloc failed"), NULL);
	}
	return (new_token);
}

/* Parse la string line et decoup selon les regles de token reco. 
Permet d'obtenir une RAW liste of tokens. */
int	ft_tokener(t_token **token_lst, char *line)
{
	t_token_type	curr_type;
	int				start;
	int				len;
	t_token			*new_token;

	start = 0;
	while (line && line[start])
	{
		len = 1;
		ft_char_type(&curr_type, line[start]);
		if (curr_type != blank)
		{
			len = ft_delimit_token(line + start, &curr_type);
			new_token = ft_create_token(line, start, len, curr_type);
			if (!new_token)
			{
				ft_tokenlst_free(token_lst);
				return (perror("token_delimiter malloc failed"), FAILURE);
			}
			ft_tokenlst_addback(token_lst, new_token);
		}
		start += len;
	}
	return (SUCCESS);
}

/*________ Parse tokens __________*/

int	ft_quotes_check(char *str)
{
	int				i;
	t_quote_type	inquote;

	i = 0;
	inquote = 0;
	while (str && str[i])
	{
		if (str[i] == '"' && (inquote != singleq || inquote == 0))
			inquote = inquote ^ doubleq;
		if (str[i] == '\'' && (inquote != doubleq || inquote == 0))
			inquote = inquote ^ singleq;
		i++;
	}
	if (inquote)
		return (ft_putstr_fd("Error: quoted field never ends\n", 2), FAILURE);
	return (SUCCESS);
}

void	ft_operator_type(t_token *elem)
{
	(void)elem;
}

int	ft_token_order(t_token *lst)
{
	t_token_type	prev_type;

	prev_type = none;
	while (lst)
	{
		if (lst->type == op && prev_type == op)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
			ft_putstr_fd(lst->str, 2);
			ft_putstr_fd("'\n", 2);
			return (FAILURE);
		}
		prev_type = lst->type;
		lst = lst->next;
	}
	return (SUCCESS);
}

int	ft_parse_tokens(t_token *lst)
{
	if (ft_tokenlst_iteri(lst, &ft_quotes_check) == FAILURE)
		return (FAILURE);
	if (ft_token_order(lst) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	t_token	*token_lst;

	token_lst = NULL;
	if (!av[1])
		return (printf("Need 1 argument str pour tester debile\n"), 1);
	printf("AV[1]=%s\n", av[1]);
	ft_tokener(&token_lst, av[1]);
	if (!token_lst)
		return (printf("Raw token list empty\n"), 1);

	if (ft_parse_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(&token_lst), 1);

	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s; type=%d\n", head->str, head->type);
	ft_tokenlst_free(&token_lst);
	return (0);
}
