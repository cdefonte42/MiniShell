/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/22 17:28:36 by mbraets          ###   ########.fr       */
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

int	ft_exec_bin(t_minishell *msh, t_cmde *cmde)
{
	char	**raw_cmde;
	int		ret_stat;

	ret_stat = 0;
	if (!cmde || !cmde->cmde_line)
		return (ret_stat);
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
	raw_cmde = NULL;
	return (ret_stat);
}

char	**ft_varlst_tochar(t_var *varlst)
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
			return (perror("varlst_tochar failed"), ft_free_tabtab(env), NULL);
		i++;
		varlst = varlst->next;
	}
	env[i] = NULL;
	return (env);
}

int	ft_token_isredir(t_token_type type)
{
	if (type == redirin || type == redirout || type == redirapp)
		return (1);
	return (0);
}
/*
heredoc = , O_CREAT | O_WRONLY | O_TRUNC, 00644		=> Pour readline
heredoc = O_RDONLY, 00644							=> pour le lire
out		= , O_WRONLY | O_CREAT | O_TRUNC = 01101
 S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH = 00644
in		= , O_RDONLY
app		= O_APPEND | O_CREAT | O_WRONLY = 02101
*/

int	ft_open(int *fd, char *pathname, int flags, int mode)
{
	*fd = open(pathname, flags, mode);
	if (*fd == -1)
		return (ft_perror(NULL, pathname), FAILURE);
	return (SUCCESS);

}
int	ft_redir(t_cmde *cmde)
{
	t_token *head_token;
	char	*file;

	if (!cmde || !cmde->cmde_line)
		return (SUCCESS);
	head_token = cmde->cmde_line;
	while (head_token)
	{
		if (ft_token_isredir(head_token->type))
			file = head_token->next->str;
		if (file && *file == 0)
			return (ft_error(file, "ambiguous redirect\n"), FAILURE);
		else
		{
			if (head_token->type == redirin)
				if (ft_open(&(cmde->fdin), file, O_RDONLY, 0) == FAILURE)
					return (FAILURE);
			if (head_token->type == redirout)
				if (ft_open(&(cmde->fdout), file, 01101, 00644) == FAILURE)
					return (FAILURE);			
			if (head_token->type == redirapp)
				if (ft_open(&(cmde->fdout), file, 02101, 00644) == FAILURE)
					return (FAILURE);
		}
		head_token = head_token->next;
	}
	return (SUCCESS);
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
		if (ft_redir(cmde) == FAILURE)
			exit(FAILURE);
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
		ft_msh_clear(msh);
		ft_free_tabtab(envp);
		exit(FAILURE);
	}
	return (SUCCESS);
}

int	ft_exec(t_minishell *msh, t_cmde *cmde)
{
	if (!cmde || !cmde->cmde_line)
		return (SUCCESS);
	if (ft_pipe_cmdes(cmde, cmde->next) == FAILURE)
		return (FAILURE);
	if ((cmde->next || cmde->prev) || !ft_isbin(cmde->cmde_line->str))
	{
		if (ft_fork(msh, cmde) == FAILURE)
			return (FAILURE);
	}
	else
		ft_exec_bin(msh, cmde); //ATTENTION nedd return status MAIS pas utiliser FAILURE ni SUCCESS
//		if (ft_exec_bin(msh, cmde) == FAILURE)
//			return (FAILURE);
	return (SUCCESS);
}

int	g_status;

void	signal_handler(int signalid)
{
	if (signalid == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
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
	t_cmde	*curr_cmde;

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
				curr_cmde = msh->cmde_lst;
				while (curr_cmde)
				{
					//ft_print_cmdelst(curr_cmde);
					ft_expansion(curr_cmde, msh->vars);
					ft_exec(msh, curr_cmde);
					curr_cmde = curr_cmde->next;
				}
				ft_cmdelst_clear(msh->cmde_lst);
				msh->cmde_lst = NULL;
			}
		}
		if (line && *line)
			add_history (line);
		free(line);
		line = NULL;
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
	ft_msh_clear(&msh);
	return (g_status);
}
