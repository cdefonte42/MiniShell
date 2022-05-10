/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 18:45:29 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/10 10:03:36 by cdefonte         ###   ########.fr       */
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

int	set_g_status(void)
{
	if (errno == 12 || errno == 5)
		return (-1);
	else
		return (1);
}

int	ft_exec_bin(t_minishell *msh, t_cmde *cmde)
{
	char	**raw_cmd;
	int		ret_stat;

	ret_stat = 0;
	errno = 0;
	if (!cmde || !cmde->cmde_line)
		return (ret_stat);
	if (ft_redir(cmde) == FAILURE)
		return (set_g_status());
	raw_cmd = ft_lst_to_char(cmde->cmde_line);
	if (!raw_cmd)
		return (-1);
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
		ret_stat = ft_pwd(cmde->pipefd[w_end]);
	else if ((ft_strcmp(raw_cmd[0], "export") == 0))
		ret_stat = ft_export(&(msh->vars), raw_cmd, cmde->pipefd[w_end]);
	else if ((ft_strcmp(raw_cmd[0], "env") == 0))
		ret_stat = bd_env(msh->vars, raw_cmd, cmde->pipefd[w_end]);
	else if ((ft_strcmp(raw_cmd[0], "unset") == 0))
		ret_stat = ft_unset(&(msh->vars), raw_cmd);
	else if ((ft_strcmp(raw_cmd[0], "echo") == 0))
		ret_stat = minishell_echo(msh, raw_cmd, cmde->pipefd[w_end]);
	free(raw_cmd);
	raw_cmd = NULL;
	return (ret_stat);
}

void	print_env(char **env)
{
	int	i = 0;

	while (env && env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
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
			printf("%s %s %s\n", env[i], varlst->key, varlst->value);
			i++;
		}
		varlst = varlst->next;
	}
	env[i] = NULL;
	return (env);
}

void	ft_exit_child(t_child child, t_minishell *msh, t_cmde *cmde, bool error)
{
	if (error)
	{
		if (errno != 2)
			ft_perror(cmde->name_token->str, NULL);
		else
			ft_error(cmde->name_token->str, "command not found");
	}
	free(child.argv);
	free(child.pathname);
	ft_free_tabtab(child.envp);
	ft_msh_clear(msh);
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
			ft_exit_child(child, msh, cmde, false);
		if (ft_dup(cmde) == FAILURE)
			ft_exit_child(child, msh, cmde, true);
		if (ft_isbin(cmde->name_token->str))
		{
			g_status = ft_exec_bin(msh, cmde);
			ft_msh_clear(msh);
			exit(g_status);
		}
		child.argv = ft_lst_to_char(cmde->cmde_line);
		if (!child.argv)
			ft_exit_child(child, msh, cmde, true);
		child.envp = ft_varlst_tochar(msh->vars);
		if (!child.envp)
			ft_exit_child(child, msh, cmde, true);
		child.pathname = check_permission(msh, child.argv[0]);
		if (!child.pathname)
		{
			ft_putstr_fd("MAIN NO CHILD PATHNAME\n", 2);
			ft_exit_child(child, msh, cmde, true);
		}
		if (cmde->next && close(cmde->next->pipefd[r_end]) == -1)
			perror("_2_mope closing in ds fork");
		execve(child.pathname, child.argv, child.envp);
		ft_exit_child(child, msh, cmde, true);
		exit(FAILURE);
	}
	if (cmde->pipefd[w_end] != 1 && close(cmde->pipefd[w_end]) == -1)
		perror("closing pipeout ft_forkout failed");
	if (cmde->pipefd[r_end] != 0 && close(cmde->pipefd[r_end]) == -1)
		perror("closing pipin ft_forkout failed");
	return (SUCCESS);
}

int	ft_exec(t_minishell *msh, t_cmde *cmde)
{
	if (!cmde || !cmde->cmde_line)
		return (SUCCESS);
	if ((cmde->next || cmde->prev) || !ft_isbin(cmde->name_token->str))
	{
		if (ft_pipe_cmdes(cmde, cmde->next) == FAILURE)
			return (FAILURE);
		if (ft_fork(msh, cmde) == FAILURE)
			return (perror("ft_fork failed ds ft_exec"), FAILURE);
	}
	else
		g_status = ft_exec_bin(msh, cmde);
	if (g_status == -1)
		return (FAILURE);
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
		return (WTERMSIG(status) + 128);
	return (0);
}

int	minishell_loop(t_minishell *msh)
{
	char	*line;
	t_cmde	*curr_cmde;

	while (msh->loop)
	{
		signal(SIGINT, &signal_handler);
		line = readline("minishell-$ ");
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
					ft_expand_tokens(curr_cmde, msh->vars);
					if (ft_tokenlst_iteri_s(curr_cmde->cmde_line, &remove_quote) == FAILURE)
						return (FAILURE); //NON
					if (ft_exec(msh, curr_cmde) == FAILURE)
						return (free(line), clear_history(), FAILURE);
					if (curr_cmde->next == NULL && curr_cmde->pid != -1)
					{
						g_status = ft_wait_cmde(curr_cmde->pid, 0);
						t_cmde	*prev;
						prev = curr_cmde->prev;
						while (prev)
						{
							ft_wait_cmde(prev->pid, 0);
							prev = prev->prev;
						}
					}
					curr_cmde = curr_cmde->next;
				}
				ft_cmdelst_clear(msh->cmde_lst);
				msh->cmde_lst = NULL;
			}
			if (g_status == -1)
				return (free(line), clear_history(), FAILURE);
			else
				ft_cmdelst_clear(msh->cmde_lst);
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
	t_minishell	msh;

	g_status = 0;
	(void)ac;
	(void)av;
	signal(SIGQUIT, &signal_handler);
	signal(SIGTSTP, &signal_handler);
	printf("________MSH PID = %d__________\n", getpid());
	ft_memset(&msh, 0, sizeof(t_minishell));
	msh.loop = 42;
	if (ft_init_envlst(&msh, envp) == FAILURE)
		return (ft_msh_clear(&msh), 1);
	printf("Welcome to my minishell.\n");
	if (minishell_loop(&msh) == FAILURE)
		return (ft_msh_clear(&msh), 1);
	printf("Bye.\n");
	ft_msh_clear(&msh);
	return (g_status);
}
