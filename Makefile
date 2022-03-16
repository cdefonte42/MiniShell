NAME		=	minishell

HEADER		=	includes/ libft/

LIBFT		=	libft/libft.a

CC			=	gcc

CFLAGS		=	-Werror -Wall -Wextra -g -I$(HEADER) -Llibft -lft -lreadline

SRCS		=	srcs/minishell.c

OBJS		=	$(SRCS:.c=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJS) $(LIBFT) $(HEADER)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBFT)

$(LIBFT)	:
				make gnl -C ./libft

clean		:
				rm -rf $(OBJS)
				make clean -C ./libft

fclean		:	clean
				rm $(NAME)
				make fclean -C ./libft

re			:	fclean all
