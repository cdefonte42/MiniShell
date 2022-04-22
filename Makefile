# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/18 12:15:17 by cdefonte          #+#    #+#              #
#    Updated: 2022/04/22 14:10:42 by mbraets          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

HEADER		=	includes/
DHEADER		=	-Iincludes/ -Ilibft/

LIBFT		=	libft/libft.a

CC			=	cc

#CFLAGS		=	-Werror -Wall -Wextra -fsanitize=address
CFLAGS		=	-Werror -Wall -Wextra -g3
LIBFLAGS	=	-I$(HEADER) -Ilibft -Llibft -lft -lreadline

SRCS		=	srcs/main.c \
srcs/tokens/token_checks.c \
srcs/tokens/token_cuts.c \
srcs/tokens/token_lst.c \
srcs/cmdes/cmde_lst.c \
srcs/export/export.c \
srcs/export/export_utils.c \
srcs/unset/unset.c \
srcs/builtin/echo.c \
srcs/builtin/cdbin.c \
srcs/builtin/cd_cdpath.c \
srcs/builtin/pwd.c \
srcs/parsing.c \
srcs/grammar.c \
srcs/utils.c \
srcs/free.c \
srcs/init_msh.c \
srcs/expansion.c

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
				valgrind --suppressions=.ignore_leaks --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./minishell
fun			: all
	nm -uAl $(NAME)

re			:	fclean all
.PHONY: all clean run fclean
