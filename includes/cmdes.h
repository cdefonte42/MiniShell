/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 19:59:42 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/15 12:23:46 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDES_H
# define CMDES_H

# define FAILURE 0
# define SUCCESS 1

# include "tokens.h"

typedef struct s_cmde {
	t_token			*cmde_line; //contient TOUS les tokens until pipe inclus
	char			**argv; // [0]=cmde name, reste = options et arguments
	int				pipe[2];
	struct s_cmde	*prev;
	struct s_cmde	*next;
}	t_cmde;

/*__________ T_CMDE LISTES FCTS___________*/
t_cmde	*ft_cmdelst_new(t_token *cmde_line);
void	ft_cmdelst_addback(t_cmde **alst, t_cmde *new);
t_cmde	*ft_cmdelst_last(t_cmde *lst);
void	ft_cmdelst_free(t_cmde *lst);
int		ft_cmdelst_size(t_cmde *lst);

#endif
