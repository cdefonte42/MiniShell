/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 10:26:30 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/07 14:16:10 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "export.h"

// SIGINT == 2

int	status;

void	signal_handler(int signalid)
{
	if (signalid == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		status = 130;
	}
	if (signalid == SIGQUIT || signalid == SIGTSTP)
		write(1, "\b\b  \b\b", 6);
}

void	builtin_exec(t_minishell *msh)
{
	int	i;

	if ((ft_strcmp(msh->raw_cmd[0], "exit") == 0))
	{
		if (msh->raw_cmd[1] != NULL)
			status = ft_atoi(msh->raw_cmd[1]);
		msh->loop = 0;
		ft_putendl_fd("exit", 1);
		// fexit(msh);
	}
	if ((ft_strcmp(msh->raw_cmd[0], "echo") == 0))
	{
		msh->status = minishell_echo(msh, msh->raw_cmd);
	}
	if ((ft_strcmp(msh->raw_cmd[0], "cd") == 0))
	{
		ft_cd(msh->raw_cmd[1], msh->env);
		char	*pwd;
		pwd = getcwd(NULL, 0);
		if (!pwd)
			perror("perror cd");
		else
			printf("new pwd after cd = %s\n", pwd);
		free(pwd);
	}
	if ((ft_strcmp(msh->raw_cmd[0], "export") == 0))
	{
		if (msh->raw_cmd[1] == NULL)
			ft_export(&msh->vars, NULL);
		else
		{
			i = 0;
			while (msh->raw_cmd[++i])
				ft_export(&msh->vars, msh->raw_cmd[i]);
		}
	}
}

int	minishell_get_env(t_minishell *msh, char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	msh->env = ft_calloc(sizeof(char **), i + 1);
	if (!msh->env)
		return (FAILURE);
	msh->env[i] = NULL;
	i = 0;
	while (envp && envp[i])
	{
		msh->env[i] = ft_split(envp[i], '=');
		if (msh->env[i] == NULL)
			return (minishell_free_env(msh), FAILURE); // free old env
		i++;
	}
	return (SUCCESS);
}

int	minishell_join_quote(t_minishell *msh)
{
	char	*tmp;
	int		in_quote;
	int		i;
	int		j;
	enum e_quote_type	quote_type;

	in_quote = false;
	quote_type = -1;
	i = 0;
	while (msh->raw_cmd[i])
	{
		j = 0;
		while (msh->raw_cmd[i][j])
		{
			if (msh->raw_cmd[i][j] == '"')
			{
				in_quote = !in_quote;
				quote_type = doubleq;
			}
			else if (msh->raw_cmd[i][j] == '\'')
			{
				in_quote = !in_quote;
				quote_type = singleq;
			}
			j++;
		}
		j = 0;
		while (in_quote == true && msh->raw_cmd[i + (++j)])
		{
			tmp = msh->raw_cmd[i];
			msh->raw_cmd[i] = ft_strsjoin(msh->raw_cmd[i], " ", msh->raw_cmd[i + j]);
			free(tmp);
			free(msh->raw_cmd[i + j]);
			j++;
		}
		i++;
	}
	printf("JOIN QUOTE\n");
	return (SUCCESS);
}

void	debug_print_msh(t_minishell *msh)
{
	int		i;

	i = 0;
	while (msh && msh->raw_cmd && msh->raw_cmd[i])
	{
		printf("[%d]%s\n", i, msh->raw_cmd[i]);
		i++;
	}
}

int	minishell_parse_line(t_minishell *msh, char *s)
{
	char	*line;

	line = ft_strtrim(s, " \f\t\r\v");
	msh->raw_cmd = ft_split(line, ' ');
	printf("___AVANT join quote\n");
	debug_print_msh(msh);
	minishell_join_quote(msh);
	if (!msh->raw_cmd)
		return (FAILURE);
	printf("___APRES join quote\n");
	debug_print_msh(msh);
	builtin_exec(msh);
	if (line && *line)
		add_history (line);
	minishell_free_rawcmd(msh);
	free(line);
	return (SUCCESS);
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
			minishell_parse_line(msh, line);
		}
		free(line);
	}
	clear_history();
	return (SUCCESS);
}

int	main(int ac, char *av[], char *envp[])
{
	(void)		ac;
	(void)		av;
	status = 0;
	t_minishell	msh;
	msh = (t_minishell){.loop = 42};
	// if (ft_palloc(&msh.vars, sizeof(t_var)))
	// 	return (1);
	if (minishell_get_env(&msh, envp) == FAILURE)
		return (1);
	signal(SIGINT, &signal_handler);
	signal(SIGQUIT, &signal_handler);
	signal(SIGTSTP, &signal_handler);
	printf("%p\n", &ac + 2);
	printf("%p\n", __builtin_frame_address(0));
	printf("Welcome to my minishell.\n");
	minishell_loop(&msh);
	printf("Bye.\n");
	minishell_free_env(&msh); 
	return (status);
}
