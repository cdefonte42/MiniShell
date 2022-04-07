/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:17:05 by mbraets           #+#    #+#             */
/*   Updated: 2022/04/07 15:26:29 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "minishell.h"

t_var	*var_getfromkey(t_var *var_list, char *key);
t_var	*ft_get_minkey(t_var *var_lst);
t_var	*ft_get_nextbigger(t_var *lst, t_var *prevmin, int *end);

int		ft_export(t_var **var_lst, char *str);
void	ft_lst_clear(t_var *lst);
#endif
