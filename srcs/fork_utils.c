/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:56:15 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/20 15:59:09 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

extern int	g_status;

/* retourne le nombre de oken ayant pour type 'type' */
int	ft_count_tokens_type(t_token *lst, t_token_type type)
{
	int	nbelem;

	nbelem = 0;
	while (lst)
	{
		if (lst->type == type)
			nbelem++;
		else if (type == word && lst->type >= op && lst->next)
			lst = lst->next;
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
	while (lst && lst->str && lst->type != spipe)
	{
		if (lst->type == word)
		{
			argv[i] = lst->str;
			i++;
			lst = lst->next;
		}
		else if (lst->type >= op && lst->next)
			lst = lst->next->next;
	}
	argv[i] = 0;
	return (argv);
}

char	**ft_varlst_tochar(t_var *varlst)
{
	char	**env;
	int		i;
	int		nbvar;

	if (!varlst)
		return (NULL);
	nbvar = ft_varlst_size_empty(varlst);
	env = ft_calloc(sizeof(char *), (nbvar + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (varlst || i < nbvar)
	{
		if (varlst->value)
		{
			env[i] = ft_strsjoin(varlst->key, "=", varlst->value);
			if (!env[i])
				return (perror("var_tochar failed"), ft_free_tabtab(env), NULL);
			i++;
		}
		varlst = varlst->next;
	}
	env[i] = NULL;
	return (env);
}

int	ft_wait_cmde(pid_t pid, int option)
{
	int		status;
	pid_t	w;

	status = 0;
	w = waitpid(pid, &status, option);
	if (w == -1)
		return (perror("waipid failed"), -1);
	if (WIFEXITED(status) && status != 0)
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status) && status != 0)
	{
		if (WTERMSIG(status) == SIGSEGV)
			ft_putstr_fd("Segmentation fault (core dumped)", 2);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)", 2);
		ft_putstr_fd("\n", 2);
		return (WTERMSIG(status) + 128);
	}
	return (0);
}
