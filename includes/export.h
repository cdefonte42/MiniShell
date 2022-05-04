/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:17:05 by mbraets           #+#    #+#             */
/*   Updated: 2022/05/04 12:02:28 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# ifndef MINISHELL_H
#  include "minishell.h"
# endif
# include "vars_lst.h"

/*________ UTILS ___________*/
int		ft_cat_var(t_var *var, char *key, char *value);
char	*var_getvaluefromkey(t_var *var_list, char *key);
t_var	*var_getfromkey(t_var *var_list, char *key);
int		ft_varlst_size_empty(t_var *var_lst);

/*________ PRINT EXPORT ___________*/
int		ft_put_export(t_var *var_lst, int fdout);

/*________ EXPORT ___________*/
int		ft_export(t_var **var_lst, char **argv, int fdout);

#endif
