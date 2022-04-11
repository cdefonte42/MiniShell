/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdefonte <cdefonte@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 19:59:42 by cdefonte          #+#    #+#             */
/*   Updated: 2022/04/11 15:46:33 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# define FAILURE 0
# define SUCCESS 1

typedef enum e_token_type {none, blank, word, redirin, redirout, redirapp, \
heredoc, dquoted, squoted, op, spipe, dollar} t_token_type;

typedef struct s_token {
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/*__________ LISTES ___________*/
t_token	*ft_tokenlst_new(char *str, int type);
void	ft_tokenlst_addback(t_token **alst, t_token *new);
t_token	*ft_tokenlst_last(t_token *lst);
void	ft_tokenlst_free(t_token **lst);
int		ft_tokenlst_iteri(t_token *lst, int (*f)(char *));

/*__________ UTILS ___________*/
int		ft_iscontrol_operator(char c);
int		ft_isoperator(char c);
int		ft_ismetachar(char c);
int		ft_formoperator(char c1, char c2);

#endif
