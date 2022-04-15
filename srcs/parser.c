/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/15 14:38:06 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

typedef struct s_token	t_cmde_line;

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
	while (lst && lst->type != spipe)
	{
		if (lst->type == word)
		{
			argv[i] = lst->str;
			i++;
			lst = lst->next;
		}
		else if (lst->type >= op)
			lst = lst->next->next;
	}
	return (argv);
}

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
				return (ft_cmdelst_free(*alst), ft_tokenlst_free(cmde_line), FAILURE);
			ft_cmdelst_addback(alst, new_cmde);
		}
	}
	return (SUCCESS);
}

void	ft_print_cmdelst(t_cmde *cmde_lst);

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
		if (!splited)
			return (perror("ft_init_envlst failed split"), FAILURE);
		if (ft_strcmp(splited[0], "_") == 0)
			type = shellvar;
		if (ft_new_var(&(msh->vars), splited[0], splited[1], type) == FAILURE)
			return (ft_free_tabtab(splited), FAILURE);
		free(splited);
		i++;
	}
	return (SUCCESS);
}

int	ft_pipe_cmdes(t_cmde *c1, t_cmde *c2)
{
	int	pipefd[2];

	if (!c1 || !c2)
		return (SUCCESS);
	if (pipe(pipefd) == -1)
		return (perror("Pipe failed ft_pipe_cmdes"), FAILURE);	
	c1->pipefd[out] = pipefd[out];
	c2->pipefd[in] = pipefd[in];
	if (close(pipefd[in]) == -1 || close(pipefd[out]) == -1)
		return (perror("closing pipes ft_pipe_cmdes"), FAILURE);
	return (SUCCESS);
}

int	ft_exec_bin(t_var **vars, t_cmde *cmde)
{
	char	**raw_cmde;
	int		ret_stat;

	ret_stat = 0;
	raw_cmde = ft_lst_to_char(cmde->cmde_line);
	if (!raw_cmde)
		return (FAILURE);
	if ((ft_strcmp(raw_cmde[0], "cd") == 0))
		ret_stat = ft_cd(vars, raw_cmde);
	else if ((ft_strcmp(raw_cmde[0], "pwd") == 0))
		ret_stat = ft_pwd();
	else if ((ft_strcmp(raw_cmde[0], "export") == 0))
		ret_stat = ft_export(vars, raw_cmde);
	else if ((ft_strcmp(raw_cmde[0], "unset") == 0))
		ret_stat = ft_unset(vars, raw_cmde);
	return (ret_stat);
}


char **ft_varlst_tochar(t_var *varlst)
{
	char	**env;
	int		i;
	int		nbvar;
	
	if (!varlst)
		return (NULL);
	nbvar = ft_varlst_size(varlst);
	env = malloc(sizeof(char *) * (nbvar + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (i < nbvar)
	{
		env[i] = ft_strjoin(varlst->key, varlst->value);
		if (!env[i])
			return (perror("ft_varlst_tochar failed"), ft_free_tabtab(env), NULL);
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	ft_exec(t_var **vars, t_cmde **cmde_lst)
{
	t_cmde	*cmde;
	
	cmde = *cmde_lst;
	if (!cmde)
		return (SUCCESS);
	if (ft_pipe_cmdes(cmde, cmde->next) == FAILURE)
		return (FAILURE);
	if (cmde->next || !ft_isbin(cmde->cmde_line->str))
		cmde->pid = fork();
	if (cmde->pid == -1)
		return (peror("fork failed ft_exec"), FAILURE);
	if (cmde->pid == 0)
	{
		if (ft_isbin(cmde->cmde_line->str))
			return (ft_exec_bin(vars, cmde));
		else
		{
			char	**raw;
			char	**env;
			raw = ft_lst_to_char(cmde->cmde_line);
			if (!raw)
				exit (EXIT_FAILURE);
			env = ft_varlst_tochar(*vars);
			if (!env)
				exit (EXIT_FAILURE);
			if (execve(raw[0], raw, env) == -1)
			{
				ft_free_tabtab(raw);
				ft_free_tabtab(env);
				ft_putstr_fd("Execve failed pour ft_exec\n", 2);
				exit (EXIT_FAILURE);	
			}
		}
	}
	return (SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_token	*token_lst;
	t_cmde	*cmde_lst;
	t_minishell msh;

	(void)ac;
	(void)av;
	token_lst = NULL;
	cmde_lst = NULL;
	msh = (t_minishell){.loop = 42};
	if (ft_init_envlst(&msh, envp) == FAILURE)
		return (1);
	if (!av[1])
		return (printf("Need 1 argument str pour tester debile\n"), 1);
	printf("Readline return equivqlent AV[1]=%s\n", av[1]);

	if (ft_tokener(&token_lst, av[1]) == FAILURE)
		return (1);
	else if (!token_lst)	
		return (printf("Raw token list empty\n"), 0);
	if (ft_check_tokens(token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), 1);


	if (ft_fill_cmdelst(&cmde_lst, token_lst) == FAILURE)
		return (ft_tokenlst_free(token_lst), 1);
	msh.cmde_lst = cmde_lst;
	ft_exec(&(msh.vars), &cmde_lst);
	ft_print_cmdelst(cmde_lst);

	ft_cmdelst_free(cmde_lst);
	return (0);
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
