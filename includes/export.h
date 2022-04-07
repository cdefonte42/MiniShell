/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:17:05 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/07 18:50:31 by mbraets          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# ifndef MINISHELL_H
#  include "minishell.h"
# endif

t_var	*var_getfromkey(t_var *var_list, char *key);
t_var	*ft_get_minkey(t_var *var_lst);
t_var	*ft_get_minkey_prev(t_var *var_lst, t_var *prev);
t_var	*ft_get_nextbigger(t_var *lst, t_var *prevmin, int *end);
void	ft_lst_clear(t_var *lst);

int		ft_export(t_var **var_lst, char *str);
void	ft_lst_clear(t_var *lst);
void	var_add_back(t_var **alst, t_var *new);
int		ft_new_var(t_var **var_lst, char *key, char *value, int type);
#endif
