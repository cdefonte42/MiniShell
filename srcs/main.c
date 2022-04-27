/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/27 15:33:45 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "export.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>

int	g_status;

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
	char	**raw_cmd;
	int		ret_stat;

	ret_stat = 0;
	if (!cmde || !cmde->cmde_line)
		return (ret_stat);
	if (ft_redir(cmde) == FAILURE)
		return (FAILURE);
//	if (cmde->pipefd[in] != 0 && close(cmde->pipefd[in]) == -1)
//		printf("NOPE CLODE PIPE IN\n");
	raw_cmd = ft_lst_to_char(cmde->cmde_line);
	if (!raw_cmd)
		return (FAILURE);
	if ((ft_strcmp(raw_cmd[0], "exit") == 0))
	{
		if (raw_cmd[1] != NULL)
		{
			if (raw_cmd[2])
				return (ft_error("too many arguments", NULL), 2);
			if (ft_stris(raw_cmd[1], ft_isdigit))
				ret_stat = ft_atoi(raw_cmd[1]);
			else
				return (ft_error("numeric argument required", NULL), 2);
		}
		ft_putendl_fd("exit", 1);
		msh->loop = 0;
	}
	else if ((ft_strcmp(raw_cmd[0], "cd") == 0))
		ret_stat = ft_cd(&(msh->vars), raw_cmd);
	else if ((ft_strcmp(raw_cmd[0], "pwd") == 0))
		ret_stat = ft_pwd(cmde->pipefd[out]);
	else if ((ft_strcmp(raw_cmd[0], "export") == 0))
		ret_stat = ft_export(&(msh->vars), raw_cmd, cmde->pipefd[out]);
	else if ((ft_strcmp(raw_cmd[0], "unset") == 0))
		ret_stat = ft_unset(&(msh->vars), raw_cmd);
	else if ((ft_strcmp(raw_cmd[0], "echo") == 0))
		ret_stat = minishell_echo(msh, raw_cmd, cmde->pipefd[out]);
	free(raw_cmd);
	raw_cmd = NULL;
//	if (cmde->pipefd[out] != 1 && close(cmde->pipefd[out]) == -1)
//		printf("NOPE CLODE PIPE OUT\n");
	return (ret_stat);
}

char	**ft_varlst_tochar(t_var *varlst)
{
	char	**env;
	int		i;
	int		nbvar;

	if (!varlst)
		return (NULL);
	nbvar = ft_varlst_size_empty(varlst);
	env = malloc(sizeof(char *) * (nbvar + 1));
	if (!env)
		return (NULL);
	i = 0;
	while (i < nbvar)
	{
		if (varlst->value)
		{
			env[i] = ft_strsjoin(varlst->key, "=", varlst->value);
			if (!env[i])
				return (perror("varlst_tochar failed"), ft_free_tabtab(env), NULL);
		}
		i++;
		varlst = varlst->next;
	}
	env[i] = NULL;
	return (env);
}

void	ft_exit_child(t_child	child, t_minishell *msh, t_cmde *cmde)
{
	if (errno != 2)
		ft_perror(cmde->cmde_line->str, NULL);
	else
		ft_error(cmde->cmde_line->str, "command not found");
	free(child.argv);
	free(child.pathname);
	ft_msh_clear(msh);
	ft_free_tabtab(child.envp);
	if (errno == 13)
		exit(126);
	else if (errno == 2 || errno == 36)
		exit(127);
	else if (errno == 5)
		exit(254);
	exit(EXIT_FAILURE);
}

int	ft_fork(t_minishell *msh, t_cmde *cmde)
{
	t_child	child;

	cmde->pid = fork();
	if (cmde->pid == -1)
		return (perror("fork failed"), FAILURE);
	ft_memset(&child, 0, sizeof(t_child));
	errno = 0;
	if (cmde->pid == 0)
	{
		if (ft_redir(cmde) == FAILURE)
			ft_exit_child(child, msh, cmde);
		if (ft_dup(cmde) == FAILURE)
			ft_exit_child(child, msh, cmde);
		if (ft_isbin(cmde->cmde_line->str))
		{
			g_status = ft_exec_bin(msh, cmde);
			ft_msh_clear(msh);
			exit(g_status);
		}
		child.argv = ft_lst_to_char(cmde->cmde_line);
		if (!child.argv)
			ft_exit_child(child, msh, cmde);
		child.envp = ft_varlst_tochar(msh->vars);
		if (!child.envp)
			ft_exit_child(child, msh, cmde);
		child.pathname = check_permission(msh, child.argv[0]);
		if (!child.pathname)
			ft_exit_child(child, msh, cmde);
		if (execve(child.pathname, child.argv, child.envp) == -1)
			perror("execve failed\n");
		ft_exit_child(child, msh, cmde);
		exit(FAILURE);
	}
	if (cmde->pipefd[in] != 0 && close(cmde->pipefd[in]) == -1)
		perror("NIEH 1");
	if (cmde->pipefd[out] != 1 && close(cmde->pipefd[out]) == -1)
		perror("NIEH 2");
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
			return (perror("ft_fork failed ds ft_exec"), FAILURE);
	}
	else
		g_status = ft_exec_bin(msh, cmde);
	printf("G_STATUS=%d\n", g_status);
	return (SUCCESS);
}

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
		signal(SIGINT, &signal_handler);
		signal(SIGQUIT, &signal_handler);
		signal(SIGTSTP, &signal_handler);
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
					ft_expansion(curr_cmde, msh->vars);
					if (ft_tokenlst_iteri_s(curr_cmde->cmde_line, &remove_quote) == FAILURE)
						return (FAILURE); //NON
					if (ft_exec(msh, curr_cmde) == FAILURE)
						return (free(line), clear_history(), FAILURE);
					if (curr_cmde->next == NULL && curr_cmde->pid != -1)
					{
						waitpid(curr_cmde->pid, &g_status, 0);
						g_status = (g_status & 0xff00) >> 8;
					}
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
	printf("Welcome to my minishell.\n");
	minishell_loop(&msh);
	printf("Bye.\n");
	ft_msh_clear(&msh);
	return (g_status);
}
