/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/21 19:35:57 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokens.h"
#include "cmdes.h"
#include "export.h"
//#include "varenv.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

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
	env = malloc(sizeof(char *) * (nbvar + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (i < nbvar)
	{
		env[i] = ft_strsjoin(varlst->key, "=", varlst->value);
		if (!env[i])
			return (perror("ft_varlst_tochar failed"), ft_free_tabtab(env), NULL);
		i++;
		varlst = varlst->next;
	}
	env[i] = NULL;
	return (env);
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

int	g_status;

void	signal_handler(int signalid)
{
	if (signalid == SIGINT)
	{
		write(1, "\n", 1);
		//rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_status = 130;
	}
	if (signalid == SIGQUIT || signalid == SIGTSTP)
		write(1, "\b\b  \b\b", 6);
}

int	minishell_loop(t_minishell *msh)
{
	char	*line;

	while (msh->loop)
	{
		line = readline(C_BLUE"minishell-"VERSION C_RESET"$ ");
		if (line == NULL)
		{
			ft_putendl_fd("exit", 1);
			break ;
		}
		else
		{
			if (ft_parse(msh, line) != FAILURE)
			{
				//ft_expansion(&(msh->cmde_lst), msh->vars);
				while (msh->cmde_lst)
				{
					ft_print_cmdelst(msh->cmde_lst);
					ft_exec(msh, msh->cmde_lst);
					t_cmde	*tmp;
					tmp = msh->cmde_lst;
					msh->cmde_lst = msh->cmde_lst->next;
					free(tmp->cmde_line->str);
					free(tmp->cmde_line);
					free(tmp);
				}
				//ft_cmdelst_clear(msh->cmde_lst);
				msh->cmde_lst = NULL;
			}
		}
		free(line);
	}
	clear_history();
	return (SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell msh;
	g_status = 0;

	(void)ac;
	(void)av;
	ft_memset(&msh, 0, sizeof(t_minishell));
	msh.loop = 42;
	if (ft_init_envlst(&msh, envp) == FAILURE)
		return (1);
	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, &signal_handler);
	signal(SIGTSTP, &signal_handler);
	printf("Welcome to my minishell.\n");
	minishell_loop(&msh);
	printf("Bye.\n");
	return (g_status);
}
