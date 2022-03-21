# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/18 12:15:17 by cdefonte          #+#    #+#              #
#    Updated: 2022/03/21 16:45:13 by mbraets          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell

HEADER		=	includes/minishell.h
DHEADER		=	-Iincludes/ -Ilibft/

LIBFT		=	libft/libft.a

CC			=	cc

CFLAGS		=	-Werror -Wall -Wextra -g3
LIBFLAGS	=	-I$(HEADER) -Ilibft -Llibft -lft -lreadline

SRCS		=	srcs/minishell.c

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

re			:	fclean all
.PHONY: all clean run fclean
