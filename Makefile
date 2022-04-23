# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbraets <mbraets@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/18 12:15:17 by cdefonte          #+#    #+#              #
#    Updated: 2022/04/23 15:25:55 by cdefonte         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	minishell


SRC_DIR = srcs/
OBJ_DIR = ./obj/
INC_DIR = includes/
LFT_DIR = libft/

INCLUDES		=	-I$(INC_DIR) -I$(LFT_DIR)

LIBFT		=	libft/libft.a

CC			=	cc

#CFLAGS		=	-Werror -Wall -Wextra -fsanitize=address
CFLAGS		=	-Werror -Wall -Wextra -g3
LIBFLAGS	=	$(INCLUDES) -Llibft -lft -lreadline

SRCS		=	srcs/main.c \
srcs/tokens/token_checks.c \
srcs/tokens/token_cuts.c \
srcs/tokens/token_lst.c \
srcs/tokens/token_utils.c \
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
srcs/redir.c \
srcs/init_msh.c \
srcs/expansion.c

#OBJS		=	$(SRCS:.c=.o)
OBJS = $(patsubst $(dir $(SRCS))%.c, $(OBJ_DIR)%.o,$(SRCS))

all			:	$(NAME)

$(NAME)		:	$(OBJS) $(LIBFT) $(INC_DIR)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT) $(LIBFLAGS)

$(LIBFT)	:
				make gnl -C ./libft

$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(INC_DIR)
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

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
