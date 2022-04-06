/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 11:51:24 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/06 15:51:38 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	new_tokens(t_list **token_lst, char *s, int start_token, int i)
{
	t_list	*newtoken;
	char	*tmp;

	newtoken = NULL;
	if (i - start_token == 0)
		return (SUCCESS);
	tmp = ft_substr(s, start_token, i - start_token);
	if (ft_strchr(tmp, '$'))
		;; //Do parameter explantion
	if (!tmp)
		return (printf("substr failed\n"), FAILURE); //et free stuff
	newtoken = ft_lstnew(tmp);
	if (!newtoken)
		return (printf("lstnew failed\n"), free(tmp), FAILURE);
	ft_lstadd_back(token_lst, newtoken);
	return (SUCCESS);
}

int	ft_get_tokens(t_list **token_lst, char *s)
{
	int					i;
	int					start_token;
	enum e_quote_type	inquote;

	i = 0;
	inquote = 0;
	start_token = 0;
	if (s == NULL)
		return (printf("readline s nulle\n"), SUCCESS);
	while (s && s[i])
	{
		if (s[i] == '"' && (inquote != singleq || inquote == 0))
			inquote = inquote ^ doubleq;
		if (s[i] == '\'' && (inquote != doubleq || inquote == 0))
			inquote = inquote ^ singleq;
		if (ft_ismetachar(s[i]) && !inquote)
		{
			if (!new_tokens(token_lst, s, start_token, i))
				return (FAILURE);
			start_token = i + 1;
			if (*ft_substr(s, i, 1) != ' ')
				ft_lstadd_back(token_lst, ft_lstnew(ft_substr(s, i, 1))); // add < or | or >
		}
		i++;
	}
	if (!new_tokens(token_lst, s, start_token, i))
		return (FAILURE);
	if (inquote)
		printf("Error.\n");
	return (SUCCESS);
}

int	main(int ac, char **av)
{
	(void)ac;
	char	*s = "   cd|Bonjour\"Tst\"\"\" \"test\"\"et";
	t_list	*token_lst;
	(void)s;
	token_lst = NULL;
	ft_get_tokens(&token_lst, av[1]);
	for (t_list *head = token_lst; head != NULL; head = head->next)
		printf("%s\n", (char *)head->content);
	return (0);
}
