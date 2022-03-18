/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lists_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 10:32:34 by cdefonte          #+#    #+#             */
/*   Updated: 2022/02/21 12:52:38 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_cmde	*ft_newcmde(int num, pid_t pid, char *argvi, char *filename)
{
	t_cmde	*new;

	new = malloc(sizeof(t_cmde));
	if (!new)
		return (NULL);
	new->num = num;
	new->pid = pid;
	new->argvi = argvi;
	new->filename = filename;
	new->pipe[in] = -1;
	new->pipe[out] = -1;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

/* Free les elements de la liste, PAS LEUR CONTENU */
void	ft_lstclear(t_cmde **lst)
{
	t_cmde	*tmp_next;

	if (!lst || !*lst)
		return ;
	while (lst && *lst && (*lst)->prev)
		*lst = (*lst)->prev;
	while (lst && *lst)
	{
		tmp_next = (*lst)->next;
		free(*lst);
		*lst = tmp_next;
	}
}

void	ft_lstadd_back(t_cmde **alst, t_cmde *new)
{
	t_cmde	*tmp;

	tmp = *alst;
	if (*alst == NULL)
		*alst = new;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new;
		new->prev = tmp;
	}
}

int	ft_init_cmdes(t_cmde **cmde, int argc, char **argv)
{
	int		nb_cmdes;
	int		i;
	t_cmde	*new_cmde;

	nb_cmdes = argc - 3;
	i = 0;
	while (i < nb_cmdes)
	{
		if (i == 0)
			new_cmde = ft_newcmde(i, 0, (argv + 2)[i], argv[1]);
		else if (i == nb_cmdes - 1)
			new_cmde = ft_newcmde(i, 0, (argv + 2)[i], argv[argc - 1]);
		else
			new_cmde = ft_newcmde(i, 0, (argv + 2)[i], (void *)0);
		if (!new_cmde)
			return (ft_lstclear(cmde), -1);
		ft_lstadd_back(cmde, new_cmde);
		i++;
	}
	return (0);
}
