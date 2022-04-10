/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 19:59:42 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/10 21:13:26 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

typedef enum e_token_type {none, word, redirin, redirout, redirapp, heredoc, \
dquoted, squoted, op, cpipe, dollar} t_token_type;

typedef struct s_token {
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

t_token	*ft_tokenlst_new(char *str, int type);
void	ft_tokenlst_addback(t_token **alst, t_token *new);
t_token	*ft_tokenlst_last(t_token *lst);
void	ft_tokenlst_free(t_token **lst);

#endif
