/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_lst.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/04 11:53:10 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/04 11:57:15 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARS_LST_H
# define VARS_LST_H

typedef enum e_var_type {envvar, shellvar}	t_var_type;

typedef struct s_var {
	char			*key;
	char			*value;
	t_var_type		type;
	struct s_var	*next;
}	t_var;

void	var_add_back(t_var **alst, t_var *new);
int		ft_new_var(t_var **var_lst, char *key, char *value, int type);
void	ft_set_var(t_var *var, char *key, char *value, int type);
void	ft_varlst_clear(t_var *lst);
int		ft_varlst_size(t_var *var_lst);

#endif
