/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:28:17 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/08 12:21:20 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define C_RESET "\033[0m"
# define C_BLUE "\033[0;34m"
# define VERSION "0.1"
# define FAILURE 0
# define SUCCESS 1
# ifndef DEBUG
#  define DEBUG 0
# endif

# include "libft.h" 

# include <stdbool.h>

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

enum e_quote_type {doubleq = 1, singleq = 2};
enum e_var_type {envvar, shellvar};

typedef struct s_cmde {
	char			*name;
	char			*option;
	char			*arg;
	int				infile;
	int				outfile;
	struct s_cmde	*next;
}	t_cmde;

typedef struct s_var {
	char			*key;
	char			*value;
	int				type;
	struct s_var	*next;
}	t_var;

typedef struct s_minishell {
	char			**path;
	char			***env;
	char			**raw_cmd;
	t_cmde			*cmde_lst;
	t_var			*vars;
	t_list			*token;
	unsigned int	status;
	bool			loop;
}	t_minishell;

// Free
void	minishell_free_env(t_minishell *msh);
void	minishell_free_rawcmd(t_minishell *msh);

int		minishell_echo(t_minishell *msh, char **av);
int		ft_cd(char *directory, t_var *var_lst);
int		ft_try_cdpath(char **curpath, char *directory, char *cdpathval);
int		ft_ismetachar(char c);
int		ft_isoperator(char c);
int		ft_isname(char *str);
int		ft_islast_chstr_ch(char *str, char c);
void	ft_free_tabtab(char **tab);

#endif
