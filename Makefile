# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/18 12:15:17 by cdefonte          #+#    #+#              #
#    Updated: 2022/05/11 11:44:48 by cdefonte         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

HEADER		=	includes/
DHEADER		=	-Iincludes/ -Ilibft/

LIBFT		=	libft/libft.a

CC			=	cc

CFLAGS		=	-Werror -Wall -Wextra -g3 -fsanitize=address
#CFLAGS		=	-Werror -Wall -Wextra
LIBFLAGS	=	-I$(HEADER) -Ilibft -Llibft -lft -lreadline

SRCS		=	srcs/main.c \
srcs/tokens/token_checks.c \
srcs/tokens/token_cuts.c \
srcs/tokens/token_lst.c \
srcs/tokens/token_utils.c \
srcs/tokens/token_debug.c \
srcs/var_lst/var_lst.c \
srcs/cmdes/cmde_lst.c \
srcs/cmdes/cmde_utils.c \
srcs/builtin/export.c \
srcs/builtin/put_export.c \
srcs/builtin/export_utils.c \
srcs/builtin/unset.c \
srcs/builtin/exit_bin.c \
srcs/builtin/echo.c \
srcs/builtin/bd_env.c \
srcs/builtin/cdbin.c \
srcs/builtin/cd_cdpath.c \
srcs/builtin/pwd.c \
srcs/parsing.c \
srcs/grammar.c \
srcs/utils.c \
srcs/utils_is.c \
srcs/free.c \
srcs/pathname.c \
srcs/fork.c \
srcs/fork_utils.c \
srcs/exec_bin.c \
srcs/redir.c \
srcs/redir_utils.c \
srcs/init_msh.c \
srcs/expansion.c \
srcs/expansion_utils.c \
srcs/heredoc.c \
srcs/heredoc_utils.c

OBJS		=	$(SRCS:.c=.o)

%.o : %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ $(DHEADER)

all			:	$(NAME)

$(NAME)		:	$(OBJS) $(LIBFT) $(HEADER)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) $(LIBFLAGS)

$(LIBFT)	:
				make gnl -C ./libft

clean		:
				rm -rf $(OBJS)
				make clean -C ./libft

fclean		:	clean
				rm -f $(NAME)
				make fclean -C ./libft

debug		:
				$(eval CFLAGS+=-DDEBUG)

run			:	debug all
				valgrind --suppressions=.ignore_leaks --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --verbose --log-file=valgrind-out.txt ./minishell
fun			: all
	nm -uAl $(NAME)

malloc_test: $(OBJS) $(LIBFT)
				pwd
				$(CC) $(CFLAGS) -fsanitize=undefined -rdynamic -o $@ ${OBJS} -I$(HEADER) -Ilibft -Llibft -lft -lreadline -L. -lmallocator

re			:	fclean all

.PHONY: all clean run fclean
