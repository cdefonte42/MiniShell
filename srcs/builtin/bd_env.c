/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bd_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 10:09:52 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/03 13:19:41 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

int	check_env_args(char **cmde)
{
	int		i;

	i = 1;
	while (cmde[i])
	{
		if (!ft_strchr(cmde[i], '='))
			return (ft_error(cmde[i], "Misuse of shell builtins"), FAILURE);
		i++;
	}
	return (SUCCESS);
}

t_list	*env_exist(char *key, t_list *lst)
{
	if (!key || !lst)
		return (NULL);
	while (lst)
	{
		if (ft_strcmp(key, (char *)(lst->content)) == 0)
			return (lst->next);
		lst = lst->next->next;
	}
	return (NULL);
}

void	put_env(t_var *lst, int fd, t_list *args)
{
	t_list	*exist;

	while (lst)
	{
		if (lst->key)
			ft_putstr_fd(lst->key, fd);
		ft_putstr_fd("=", 2);
		{
			exist = env_exist(lst->key, args);
			if (exist)
				ft_putstr_fd((char *)(exist->content), 2);
			else if (lst->value)
				ft_putstr_fd(lst->value, 2);
		}
		ft_putstr_fd("\n", 2);
		lst = lst->next;
	}
}

void	free_lst_content(void *content)
{
	if (content)
	{
		free(content);
		content = NULL;
	}
}

int	get_lst_args(t_list **alst, char **args, int nb_args)
{
	t_list	*new;
	char	*content;

	while (args && --nb_args >= 0)
	{
		content = ft_strdup_until_c(args[nb_args], '=');
		if (!content)
			return (ft_lstclear(alst, &free_lst_content), FAILURE);
		new = ft_lstnew(content);
		if (!new)
			return (ft_lstclear(alst, &free_lst_content), FAILURE);
		ft_lstadd_back(alst, new);
		content = ft_strdup(ft_strchr(args[nb_args], '=') + 1);
		if (!content)
			return (ft_lstclear(alst, &free_lst_content), FAILURE);
		new = ft_lstnew(content);
		if (!new)
			return (ft_lstclear(alst, &free_lst_content), FAILURE);
		ft_lstadd_back(alst, new);
	}
	return (SUCCESS);
}

void	ft_print_cpy(t_list *cpy)
{
	while (cpy)
	{
		printf("key=%s et val=%s\n", (char *)(cpy->content), (char *)(cpy->next->content));
		cpy = cpy->next->next;
	}
}

void	lst_remove_exist(t_var *vars, t_list **alst)
{
	t_list	*prev;
	t_list	lst;
	t_vars	*head;

	if (!vars || !*alst)
		return ;
	lst = *alst;
	prev = NULL;
	while (lst)
	{
		head = vars;
		while (head)
		{
			if (ft_strcmp((char *)(lst->content), head->key) == 0)
			{
				free(lst->content)
				
			}
			head = head->next;
		}
		prev = lst;
		lst = lst->next;
	}
}

int	bd_env(t_var *var_lst, char **cmde, int fd)
{
	t_list	*cpy;
	int		nb_args;

	(void)var_lst;
	(void)fd;
	if (!cmde || !cmde[0])
		return (SUCCESS);
	if (check_env_args(cmde) == FAILURE)
		return (2);
	cpy = NULL;
	nb_args = 0;
	while (cmde[nb_args + 1])
		nb_args++;
	if (get_lst_args(&cpy, cmde + 1, nb_args) == FAILURE)
		return (-1);
	put_env(var_lst, fd, cpy);
	lst_remove_exist(var_lst, &cpy);
	ft_print_cpy(cpy);
	ft_lstclear(&cpy, &free_lst_content);
	return (0);
}

/* Tant que lst->key != args[keys], on print normal. si lst->key trouve ds 
args[keys], alors print args[value] et pas lst->value. une fois arrive a la fin
de var.lst, on print le tableau args. PB faut pas imprimer ceux deja fait etape
d'avant;*/
