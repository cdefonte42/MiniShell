/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 11:51:24 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/11 12:20:29 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "minishell.h"

/* Set la valeur du type de token en fonction du caractere c SSI le type actuel
est 'none' (default). */
void	ft_token_type(t_token_type *type, char c)
{
	if (ft_isblank(c))
		*type = blank;
	else if (ft_isoperator(c))
		*type = op;
//	else if (c == '\'')
//		*type = squoted;
//	else if (c == '"')
//		*type = dquoted;
	else
		*type = word;
}

int	ft_token_size(char *line, t_token_type *token_type)
{
	int				len;
	t_quote_type	inquote;

	len = 0;
	inquote = 0;
	if (*token_type == op)
	{
		if (ft_isoperator(line[0])
			&& ft_formoperator(line[0], line [1]))
			return (2);
		else if (line[0] == '&' && line[1] != '&')
			*token_type = word;
		return (1);
	}
	while (line && line[len])
	{
		if (line[len] == '"' && (inquote != singleq || inquote == 0))
			inquote = inquote ^ doubleq;
		if (line[len] == '\'' && (inquote != doubleq || inquote == 0))
			inquote = inquote ^ singleq;
		if (ft_ismetachar(line[len]) && !inquote)
			return (len);
		len++;
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
int	ft_token_delimiter(t_token **token_lst, char *line)
{
	t_token_type	curr_type;
	int				start;
	int				len;
	t_token			*new_token;

	start = 0;
	while (line && line[start])
	{
		len = 1;
		ft_token_type(&curr_type, line[start]);
		if (curr_type != blank)
		{
			len = ft_token_size(line + start, &curr_type);
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

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	t_token	*token_lst;

	token_lst = NULL;
	if (!av[1])
		return (printf("Need 1 argument str pour tester debile\n"), 1);
	printf("AV[1]=%s\n", av[1]);
	ft_token_delimiter(&token_lst, av[1]);
	if (!token_lst)
		return (printf("Raw token list empty\n"), 1);
	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s; type=%d\n", head->str, head->type);
	ft_tokenlst_free(&token_lst);
	return (0);
}
