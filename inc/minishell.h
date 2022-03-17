/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/03 21:22:34 by cdefonte          #+#    #+#             */
/*   Updated: 2022/03/17 09:44:26 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <stddef.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include <errno.h>

typedef struct s_cmde
{
	int				num;
	pid_t			pid;
	char			*argvi;
	char			*filename;
	int				pipe[2];
	struct s_cmde	*prev;
	struct s_cmde	*next;
}				t_cmde;

enum e_stream {in, out};
enum e_token_type {command, file};

/*_______GET_PATH______*/
char	**ft_get_cmde_arg(char *argvi, char **path);
char	*ft_get_cmde_path(char *cmde, char **paths);
char	*ft_add_cmde_to_path(char *path, char *cmde);
char	**ft_get_path(char **environ);
void	ft_expansion(char ***cmde_args, char ***paths, char **environ, \
t_cmde *cmde);

/*_______UTILS______*/
void	ft_free_tab(char **tab);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(char *s);
char	*ft_strrchr(const char *s, int c);
void	ft_putstr_fd(char *s, int fd);

/*_______REDIR______*/
int		ft_file1(int *pipefd, char *filename);
int		ft_file2(int *pipefd, char *filename);
int		ft_redir_stdout_to_file(char *filename);
int		ft_redir_file_to_stdin(char *filename);
int		ft_redir_pipe_to(int fd, int pipe);

/*_______ERROR HANDLER______*/
void	ft_putstr_until_fd(char *s, char c, int fd);
int		ft_error_handler(char *bcse_of, t_cmde **cmde, int type);

/*_______SPLIT_____*/
char	**ft_split(char const *s, char c);

/*_______LISTS_OP_____*/
t_cmde	*ft_newcmde(int num, pid_t pid, char *argvi, char *filename);
void	ft_lstadd_back(t_cmde **alst, t_cmde *new);
void	ft_lstclear(t_cmde **lst);
int		ft_init_cmdes(t_cmde **cmde, int argc, char **argv);

/*_______PIPEX_________*/
int		ft_pipex_first(char **environ, t_cmde **list);
int		ft_pipex_last(char **environ, t_cmde **list);
int		ft_pipex_mid(char **environ, t_cmde **list, int argc);

/*_______EXEC_________*/
void	ft_execve(char **cmde_args, char **paths, t_cmde *cmde, char **env);
int		ft_wait_cmde(pid_t pid);
int		ft_exec_mid(char **env, t_cmde *cmde);
int		ft_exec_first(char **env, t_cmde *cmde, int (*f)(int *, char *));
int		ft_exec_last(char **env, t_cmde *cmde, int (*f)(int *, char *));

#endif
