/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 11:51:24 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/05 12:58:50 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell"

int	ft_get_tokens(t_list **token_lst, char *s)
{
	int		i;
	t_list	*newtoken;
	char	*tmp;
	int		token_start;

	i = 0;
	token_start = 0;
	newtoken = NULL;
	if (s == NULL)
		return (printf("readline s nulle\n"), SUCCESS);
	while (s && s[i])
	{
		while (s && s[i] != '\'' && s[i] != '"' && !ft_ismetachar(s[i]))
			i++;
		if (token_start - i != 0)
		{
			tmp = ft_substr(s, token_start, i - token_start);
			if (!tmp)
				return (printf("substr failed\n"), -1); //et free stuff
		}
		newtoken = ft_lstnew(&tmp);
		free(tmp);
		if (!newtoken)
			return (printf("lstnew failed\n", -1);
		ft_lstadd_back(token_lst, newtoken);
		token_start = i;
		

		i++;
	}
	return (SUCCESS);
}

int	main(void)
{
	char	*s;
	t_list	*token_lst;

	token_lst = NULL;
	s = "cd Bonjour";
	ft_get_tokens(&token_lst, s);
	return (0);
}
