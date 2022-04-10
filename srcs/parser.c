/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 11:51:24 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/10 19:32:23 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Set la valeur du type de token en fonction du caractere c SSI le type actuel
est 'none' (default). */
void	ft_token_type(t_token_type *type, char c)
{
	if (*type != none)
		return ;
	if (ft_isoperator(c))
		*type = op;
	else
		*type = word;
}

int	ft_token_delimiter(t_token **token_lst, char *line)
{
	t_token_type	curr_type;
	int		start;
	int		i; //ATTENTION overfloaw longueur line
	
	start = 0;
	curr_type = none;
	while (ft_isblank(line[start])) //discards firts blank caracteres
		start++;
	if (line && ft_iscontrol_operator(line[start]))
	{
		printf("syntax error near unexpected token `%c'\n", line[start]);
		errno = 130; //car return status doit etre = 258 (128 + 130);
		return (FAILURE);
	}
	while (line && line[start])
	{
		ft_token_type(&curr_type, line[start]);
		i = start;
		if (curr_type == op)
		{
			if (ft_isoperator(line[i++]) && ft_formoperator(line[i], line [i + 1]))
				i++;
		}
		new_token_str = ft_substr(line, start, i - start + 1);
		if (!new_token_str)
			return (perror("token_delimiter malloc"), FAILURE) // FREE TOKEN LST
		ft_tokenlst_add(token_lst, new_token_str, curr_type);
		curr_type = none;
		i++;
		while (ft_isblank(line[i])) //discards firts blank caracteres
			i++;
	}
	return (SUCCESS); //ATTENTION peut retourner SUCCESS en laissant token_lst = NULL
}

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	char	*s = "   cd|Bonjour\"Tst\"\"\" \"test\"\"et";
	(void)s;
	t_token	*token_lst;

	token_lst = NULL;
	printf("AV[1]=%s\n", av[1]);
	ft_token_delimiter(&token_lst, av[1]);
	for (t_token *head = token_lst; head != NULL; head = head->next)
		printf("%s\ntype=%d", head->str, head->type);
	return (0);
}
