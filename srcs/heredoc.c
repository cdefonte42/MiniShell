#include "tokens.h"
#include "cmdes.h"
#include "export.h"
//#include "varenv.h"
#include "minishell.h"
#include "libft.h"

extern int	g_status;

void	signal_hd(int sig)
{
	g_status = 128 + sig;
	if (sig == SIGINT)
		close(0);
}

void	ignore_sig(int q)
{
	(void) q;
}

int	ft_heredoc_input(char *delimiter, int fd, t_quote_type quoted)
{
	char			*line;

	(void)quoted;
	ft_putstr_fd("> ", 1);
	line = get_next_line(0);
	while (line && g_status != 130)
	{
		if (ft_strlen(line) > ft_strlen(delimiter) && \
		!ft_strncmp(line, delimiter, ft_strlen(line) - 1))
			break ;
//		if (quoted != nil)
//			ft_expand_str(());
		ft_putstr_fd(line, fd);
		free(line);
		line = NULL;
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (line == NULL && g_status != 130)
			ft_putstr_fd("heredoc line null\n", 2);
	}
	free(line);
	line = NULL;
	return (SUCCESS);
}

int	ft_heredoc(t_cmde *cmde, char *delimiter)
{	
	int				fd;
	t_quote_type	quoted;

	signal(SIGINT, &signal_hd);
	signal(SIGQUIT, &signal_hd);
	quoted = msh_isquoted(delimiter);
	if (remove_quote(&delimiter) == FAILURE)
		return (perror("ft_heredoc remove auote failed"), -1);
	fd = open(cmde->hdfile, O_CREAT | O_WRONLY | O_TRUNC, 00644);
	ft_heredoc_input(delimiter, fd, quoted);
	if (fd == -1)
		return (perror("ft_heredoc opening fd failed"), free(delimiter), 1);
	if (close(fd) == -1)
		perror("ft_heredoc close fd failed");
	return (g_status);
}

int	heredoc_fork(t_minishell *msh, t_cmde *cmde, char *delimiter)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (FAILURE);
	signal(SIGINT, &ignore_sig);
	if (pid == 0)
	{
		if (ft_heredoc(cmde, delimiter) == -1)
			g_status = 12;
		ft_msh_clear(msh);
		exit(g_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_status = WEXITSTATUS(status);
		if (g_status == 12)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	rand_hdname(t_cmde *cmd_lst)
{
	unsigned long	ptr;
	char			*tmppath;
	char			*tmp;

	tmppath = "/tmp/";
	ptr = (unsigned long) cmd_lst;
	if (cmd_lst->hdfile)
		return (SUCCESS);
	tmp = ft_itoa(ptr);
	if (!tmp)
		return (FAILURE);
	cmd_lst->hdfile = ft_strjoin(tmppath, tmp);
	free(tmp);
	if (!cmd_lst->hdfile)
		return (FAILURE);
	return (SUCCESS);
}
