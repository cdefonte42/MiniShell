/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:28:17 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/30 18:05:37 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define C_RESET "\033[0m"
# define C_BLUE "\033[0;34m"
# define VERSION "0.1"
# define FAILURE 0
# define SUCCESS 1
# define HEREDOC_FILE "heredoc_tmp"
# ifndef DEBUG
#  define DEBUG 0
# endif

# include "libft.h" 
# include "gnl/get_next_line.h"
# include <stdbool.h>

# include "tokens.h"
# include "cmdes.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
/*
readline(3), rl_clear_history, rl_on_new_line,
rl_replace_line, rl_redisplay, add_history,
printf, malloc, free, write, access, open, read,
close, fork, wait, waitpid, wait3, wait4, signal,
sigaction, sigemptyset, sigaddset, kill, exit,
getcwd, chdir, stat, lstat, fstat, unlink, execve,
dup, dup2, pipe, opendir, readdir, closedir,
strerror, perror, isatty, ttyname, ttyslot, ioctl,
getenv, tcsetattr, tcgetattr, tgetent, tgetflag,
tgetnum, tgetstr, tgoto, tputs
*/

typedef enum e_var_type {envvar, shellvar}	t_var_type;

typedef struct s_var {
	char			*key;
	char			*value;
	t_var_type		type;
	struct s_var	*next;
}	t_var;

typedef struct s_minishell {
	char			**raw_cmd;
	t_cmde			*cmde_lst;
	t_var			*vars;
	unsigned int	status;
	bool			loop;
}	t_minishell;

typedef struct s_child {
	char	**argv;
	char	**envp;
	char	*pathname;
}	t_child;

// Free
void	ft_msh_clear(t_minishell *msh);
void	minishell_free_rawcmd(t_minishell *msh);
int		minishell_echo(t_minishell *msh, char **av, int fdout);
int		ft_cd(t_var **var_lst, char **directory);
int		ft_pwd(int fdout);
int		ft_try_cdpath(char **curpath, char *directory, t_var *cdpath);
int		ft_ismetachar(char c);
int		ft_isoperator(char c);
int		ft_islast_chstr_ch(char *str, char c);

/*________ EXPANSION __________*/
int		ft_expand_tokens(t_cmde *cmde_elem, t_var *vars_lst);
int		expand_str(char **str, t_var *var_lst);

/*________ EXPANSION UTILS__________*/
int		get_value(char *dolls, char **value, t_var *var_lst, int inquote);
int		get_dolls(char *str, char **dolls);
int		exp_replacewith(char **src, int start, char *t_rep, char *rep_w);
void	set_curr_quote(char c, int *qtype);

/*________ REDIRECTION __________*/
int		ft_pipe_cmdes(t_cmde *c1, t_cmde *c2);
int		ft_redir(t_cmde *cmde);
int		ft_dup(t_cmde *cmde);

/*________ MSH __________*/
int		ft_init_envlst(t_minishell *msh, char **envp);

/*________ UTILS __________*/
int		ft_isname(char *str);
int		ft_cisname(char c);
int		ft_fcisname(char c);
void	ft_free_tabtab(char **tab);
int		ft_isblank(char c);
int		ft_isset(char c, char *set);
int		ft_isbin(char *name);
void	ft_perror(char *cmd, char *suffix);
void	ft_error(char *prefix, char *suffix);

/*_________ PARSE __________*/
int		ft_parse(t_minishell *msh, char *line);
void	ft_print_cmdelst(t_cmde *cmde_lst);
int		msh_isquoted(char *str);

/*_________ PATHNAME __________*/
char	*check_permission(t_minishell *msh, char *cmd);

/*_________ HERE DOC __________*/
void	signal_hd(int sig);
void	ignore_sig(int q);
int		ft_heredoc_input(char *delimiter, int fd, t_quote_type quoted);
int		ft_heredoc(t_cmde *cmde, char *delimiter);
int		heredoc_fork(t_minishell *msh, t_cmde *cmde, char *delimiter);
int		rand_hdname(t_cmde *cmd_lst);

#endif
