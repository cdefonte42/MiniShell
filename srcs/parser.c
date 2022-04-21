/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/21 17:37:32 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

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
				return (ft_cmdelst_clear(*alst), ft_tokenlst_free(cmde_line), FAILURE);
			ft_cmdelst_addback(alst, new_cmde);
		}
	}
	return (SUCCESS);
}

int	ft_init_envlst(t_minishell *msh, char **envp)
{
	int			i;
	char		*splited[2];
	t_var_type	type;
	int			j;

	i = 0;
	ft_memset(splited, 0, sizeof(char *) * 2);
	if (!envp)
		return (FAILURE);
	while (envp && envp[i])
	{
		type = envvar;
		j = 0;
		while (envp[i] && envp[i][j] != '=')
			j++;
		splited[0] = ft_strdup_until_i(envp[i], j);
		if (!splited[0])
			return (perror("ft_init_envlst failed"), FAILURE);
		splited[1] = ft_strdup_until_i(envp[i] + j, ft_strlen(envp[i] + j));
		if (!splited[1])
			return (free(splited[0]), perror("ft_init_envlst failed"), FAILURE);
		if (ft_strcmp(splited[0], "_") == 0)
			type = shellvar;
		if (ft_new_var(&(msh->vars), splited[0], splited[1], type) == FAILURE)
			return (FAILURE);
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

int	ft_exec_bin(t_minishell *msh, t_cmde *cmde)
{
	char	**raw_cmde;
	int		ret_stat;

	printf("DANS EXEC BIN\n");
	ret_stat = 0;
	raw_cmde = NULL;
	raw_cmde = ft_lst_to_char(cmde->cmde_line);
	if (!raw_cmde)
		return (FAILURE);
	if ((ft_strcmp(raw_cmde[0], "cd") == 0))
		ret_stat = ft_cd(&(msh->vars), raw_cmde);
	else if ((ft_strcmp(raw_cmde[0], "pwd") == 0))
		ret_stat = ft_pwd();
	else if ((ft_strcmp(raw_cmde[0], "export") == 0))
		ret_stat = ft_export(&(msh->vars), raw_cmde);
	else if ((ft_strcmp(raw_cmde[0], "unset") == 0))
		ret_stat = ft_unset(&(msh->vars), raw_cmde);
	else if ((ft_strcmp(raw_cmde[0], "echo") == 0))
		ret_stat = minishell_echo(msh, raw_cmde);
	free(raw_cmde);
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
	printf("LAAAAAAAAAAAAAA nb=%d\n", nbvar);
	env = malloc(sizeof(char *) * (nbvar + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (i < nbvar)
	{
		env[i] = ft_strsjoin(varlst->key, "=", varlst->value);
		printf(":%s\n", env[i]);
		if (!env[i])
			return (perror("ft_varlst_tochar failed"), ft_free_tabtab(env), NULL);
		i++;
		varlst = varlst->next;
	}
	env[i] = NULL;
	return (env);
}

void	ft_cmdslst_free(t_cmde *lst)
{
	t_cmde	*tmp;

	while (lst)
	{
		tmp = lst;
		lst = lst->next;
		free(tmp);
	}
}

int	ft_fork(t_minishell *msh, t_cmde *cmde)
{
	char	**argv;
	char	**envp;
	int		ret_status;

	cmde->pid = fork();
	if (cmde->pid == -1)
		return (FAILURE);
	if (cmde->pid == 0)
	{
		if (ft_isbin(cmde->cmde_line->str))
		{
			ret_status = ft_exec_bin(msh, cmde);
			ft_msh_clear(msh);
			exit(ret_status);
		}
		argv = ft_lst_to_char(cmde->cmde_line);
		if (!argv)
			exit(EXIT_FAILURE);
		envp = ft_varlst_tochar(msh->vars);
		if (!envp)
			exit(EXIT_FAILURE);
		printf("333 %s %s", argv[0], envp[0]);
		// execve();
		free(argv);
		ft_free_tabtab(envp);
		exit(FAILURE);
	}
	return (SUCCESS);
}

int	ft_exec(t_minishell *msh, t_cmde *cmde)
{
	if (!cmde)
		return (SUCCESS);
	if (ft_pipe_cmdes(cmde, cmde->next) == FAILURE)
		return (FAILURE);
	if ((cmde->next || cmde->prev) || !ft_isbin(cmde->cmde_line->str))
	{
		if (ft_fork(msh, cmde) == FAILURE)
			return (FAILURE);
	}
	else
		if (ft_exec_bin(msh, cmde) == FAILURE)
			return (FAILURE);
	return (SUCCESS);
}


// int	ft_exec(t_var **vars, t_cmde **cmde_lst)
// {
// 	t_cmde	*cmde;
	
// 	cmde = *cmde_lst;
// 	if (!cmde)
// 		return (SUCCESS);
// 	if (ft_pipe_cmdes(cmde, cmde->next) == FAILURE)
// 		return (FAILURE);
	// if (cmde->pid != 0 && ((cmde->next || cmde->prev) || !ft_isbin(cmde->cmde_line->str)))
	// 	cmde->pid = fork();
// 	if (cmde->pid == -1)
// 		return (perror("fork failed ft_exec"), FAILURE);
// 	if (cmde->pid == 0)
// 	{
// 		if (ft_isbin(cmde->cmde_line->str))
// 			return (ft_exec_bin(vars, cmde));
// 		else
// 		{
// 			printf("Hi%s\n", cmde->cmde_line->str);
// 			char	**raw;
// 			char	**env;
// 			raw = ft_lst_to_char(cmde->cmde_line);
// 			if (!raw)
// 				exit (EXIT_FAILURE);
// 			env = ft_varlst_tochar(*vars);
// 			if (!env)
// 				exit (EXIT_FAILURE);
// 			if (execve(raw[0], raw, env) == -1)
// 			{
// 				ft_free_tabtab(raw);
// 				ft_free_tabtab(env);
// 				ft_putstr_fd("Execve failed pour ft_exec\n", 2);
// 				exit (EXIT_FAILURE);	
// 			}
// 		}
// 	}
// 	else
// 		printf("%d %d %d\n", (cmde->next || cmde->prev) , !ft_isbin(cmde->cmde_line->str), cmde->pid);
// 	return (SUCCESS);
// }

/**
 * @brief replace every $(name rules) by is variable
 *  
 * @param cmde_lst 
 * @return int 
 */
int	ft_expansion(t_cmde **cmde_lst, t_var *vars_lst)
{
	t_cmde	*head_cmd;
	t_token	*head_token;
	int		i;
	int		len_var;
	char	*key;
	char	*newstr;

	head_cmd = *cmde_lst;
	while (head_cmd)
	{
		if (head_cmd->cmde_line)
		{
			head_token = head_cmd->cmde_line;
			while (head_token)
			{
				printf("[%d]: %s\n", head_token->type, head_token->str);
				if (head_token->type == word && ft_strchr(head_token->str, '$'))
				{
					i = 0;
					while (head_token->str && head_token->str[i])
					{
						if (head_token->str[i] == '$')
						{
							len_var = 1;
							while (ft_cisname(head_token->str[i + len_var]))
								len_var++;
							if (len_var == 1 && ++i)
								continue ;
							key = ft_substr(head_token->str, i, len_var);
							if (!key)
								return (FAILURE);
							char *value = var_getvaluefromkey(vars_lst, key + 1);
							newstr = ft_replacestr(head_token->str, key, value);
							if (!newstr)
								return (free(key), FAILURE);
							free(head_token->str);
							head_token->str = newstr;
							printf("pff %d %d %s %s\n", i, len_var, key, head_token->str);
						}
						i++;
					}
				}
				head_token = head_token->next;
			}
		}
		head_cmd = head_cmd->next;
	}
	// ft_replacestr();
	return (SUCCESS);
}

void	ft_print_cmdelst(t_cmde *cmde_lst);

int	main(int ac, char **av, char **envp)
{
	t_token	*token_lst;
	t_cmde	*cmde_lst;
	t_minishell msh;

	(void)ac;
	(void)av;
	token_lst = NULL;
	cmde_lst = NULL;
	ft_memset(&msh, 0, sizeof(t_minishell));
	msh.loop = 42;
	if (ft_init_envlst(&msh, envp) == FAILURE)
		return (ft_msh_clear(&msh), 1);
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
	ft_expansion(&cmde_lst, msh.vars);
	ft_print_cmdelst(cmde_lst);
	for (t_cmde *head = cmde_lst; head; head = head->next)
		ft_exec(&msh, head);
	ft_msh_clear(&msh);
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
