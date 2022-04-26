/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:17:05 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/26 10:55:24 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# ifndef MINISHELL_H
#  include "minishell.h"
# endif

t_var	*var_getfromkey(t_var *var_list, char *key);
char	*var_getvaluefromkey(t_var *var_list, char *key);
t_var	*ft_get_minkey(t_var *var_lst);
t_var	*ft_get_minkey_prev(t_var *var_lst, t_var *prev);
void	ft_varlst_clear(t_var *lst);
int		ft_varlst_size(t_var *var_lst);
int		ft_varlst_size_empty(t_var *var_lst);

int		ft_export(t_var **var_lst, char **argv, int fdout);
void	var_add_back(t_var **alst, t_var *new);
int		ft_new_var(t_var **var_lst, char *key, char *value, int type);

void	ft_rm_varlst_elem(t_var **lst, char *key);
int		ft_unset(t_var **lst, char **argv);

#endif
