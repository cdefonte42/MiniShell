/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 11:51:24 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/10 12:32:58 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

int	ft_init_envlst(t_minishell *msh, char **envp)
{
	int			i;
	char		**splited;
	t_var_type	type;

	i = 0;
	if (!envp)
		return (FAILURE);
	while (envp && envp[i])
	{
		type = envvar;
		splited = ft_split(envp[i], '=');
		if (ft_strcmp(splited[1], "_") == 0)
			type = shellvar;
		if (ft_new_var(&(msh->vars), splited[0], splited[1], type) == FAILURE)
			return (ft_free_tabtab(splited), FAILURE);
		free(splited);
		i++;
	}
	return (SUCCESS);
}

/* Parcours src  jusqu'a ce que ce ne soit plus un name. Alloue le name 
resultant. Retourne FAILURE en cas d'erreur de malloc seulement (peut laisser
name à NULL);
ATTENTION src = ft_strchr(s, $) + 1. Autrement dit src[0] = 1er cara de name.*/
int	ft_extract_name(char *src, char **name)
{
	int		i;

	i = 0;
	if (src && src[0] != '_' && !ft_isalpha(src[0]))
	{
		*name = NULL;
		return (SUCCESS);
	}
	i = 1;
	while (src && src[i] && (src[i] == '_' || ft_isalnum(src[i])))
		i++;
	*name = ft_substr(src, 0, i);
	if (!*name)
		return (FAILURE);
	return (SUCCESS);
}

/* Vas cherche si la variable correspondante existe, et retourne sa 
char * value. 
ATTENTION src = ft_strchr(s, $) + 1. */
int	ft_param_subst(t_var *lst, char *s, int idoll)
{
	t_var	*var;
	char	*name;
	char	*new_s;

	ft_extract_name(s + idoll + 1, &name);
	var = var_getfromkey(lst, name);
	if (!var)
	{
		new_s = malloc(sizeof(char) * (ft_strlen(s) - ft_strlen(name + 1) + 1));
	}
	return (SUCCESS);
}

//While ft_need_expansion(s) is TRUE, ft_prama_expansion
int	ft_param_expansion(t_var *var_lst, char *s)
{
	int		i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '$')
		{
			ft_param_subst(var_lst, s, i);
		}
		i++;
	}
	return (SUCCESS);
}

int	new_tokens(t_list **token_lst, char *s, int start_token, int i)
{
	t_list	*newtoken;
	char	*tmp;

	newtoken = NULL;
	if (i - start_token == 0)
		return (SUCCESS);
	tmp = ft_substr(s, start_token, i - start_token);
	if (!tmp)
		return (printf("substr failed\n"), FAILURE); //et free stuff
	newtoken = ft_lstnew(tmp);
	if (!newtoken)
		return (printf("lstnew failed\n"), free(tmp), FAILURE);
	ft_lstadd_back(token_lst, newtoken);
	return (SUCCESS);
}

int	ft_get_tokens(t_list **token_lst, char *s, t_var *var_lst)
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
			if (new_tokens(token_lst, s, start_token, i) == FAILURE)
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

int	main(int ac, char **av, char **envp)
{
	char	*s = "   cd|Bonjour\"Tst\"\"\" \"test\"\"et";
	t_list	*token_lst = NULL;
	t_minishell	msh;

	(void)ac;
	(void)av;
	(void)s;

	printf("AV[1]=%s\n", av[1]);
	msh = (t_minishell){.loop = 42};
	if (ft_init_envlst(&msh, envp) == FAILURE)
		return (1);
	ft_get_tokens(&token_lst, av[1], msh->vars);
	msh->token = token_lst;
	for (t_list *head = msh.token; head != NULL; head = head->next)
		printf("%s\n", (char *)head->content);
	for (t_list *tok = msh.token; tok != NULL; tok = tok->next)
		ft_param_expansion(msh.vars, tok);
	return (0);
}
