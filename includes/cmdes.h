/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 19:59:42 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/29 17:06:56 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDES_H
# define CMDES_H

# define FAILURE 0
# define SUCCESS 1

# include "tokens.h"
# include "unistd.h"

enum e_fd_type {r_end = 0, w_end = 1};

typedef struct s_cmde {
	t_token			*cmde_line;
	int				pipefd[2];
	pid_t			pid;
	char			*hdfile;
	struct s_cmde	*prev;
	struct s_cmde	*next;
}	t_cmde;

/*__________ T_CMDE LISTES FCTS___________*/
t_cmde	*ft_cmdelst_new(t_token *cmde_line);
void	ft_cmdelst_addback(t_cmde **alst, t_cmde *new);
t_cmde	*ft_cmdelst_last(t_cmde *lst);
void	ft_cmdelst_clear(t_cmde *lst);
int		ft_cmdelst_size(t_cmde *lst);

#endif
