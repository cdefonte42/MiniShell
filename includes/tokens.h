/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/10 19:59:42 by cdefonte          #+#    #+#             */
/*   Updated: 2022/05/16 12:40:44 by cdefonte         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
# define TOKENS_H

# define FAILURE 0
# define SUCCESS 1

typedef enum e_quote_type {nil, doubleq, singleq}	t_quote_type;
typedef enum e_token_type {none, blank, word, op, redirin, redirout, redirapp, \
heredoc, spipe}	t_token_type;

typedef struct s_token {
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/*__________ T_TOKEN LISTES FCTS___________*/
t_token	*ft_tokenlst_new(char *str, int type);
void	ft_tokenlst_addback(t_token **alst, t_token *new);
t_token	*ft_tokenlst_last(t_token *lst);
void	ft_tokenlst_free(t_token *lst);
void	ft_tokenlst_pop(t_token **lst, t_token *to_pop, t_token *prev);

/*__________ TOKEN LST UTILS ___________*/
int		ft_tokenlst_iteri(t_token *lst, int (*f)(t_token *));
int		ft_tokenlst_iteri_s(t_token *lst, int (*f)(char **));
int		ft_tokenlst_size(t_token *lst);
void	ft_remove_empty_token(t_token **token_lst);
void	ft_tokenlst_addafter(t_token *elem, t_token *new);

/*__________ UTILS ___________*/
int		ft_iscontrol_operator(char c);
int		ft_isoperator(char c);
int		ft_ismetachar(char c);
int		ft_formoperator(char c1, char c2);

/*__________ CUT TOKENS ___________*/
void	ft_char_type(t_token_type *type, char c);
int		ft_delimit_token(char *line, t_token_type *token_type);
t_token	*ft_create_token(char *line, int start, int len, int type);
int		ft_tokener(t_token **token_lst, char *line);

/*__________ CHECK TOKENS ___________*/
int		ft_quotes_check(t_token *lst);
void	ft_set_operator_type(t_token *elem); //sais pas ou la mettre
int		remove_quote(char **s);
int		ft_check_tokens(t_token *lst);

#endif
