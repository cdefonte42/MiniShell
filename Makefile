NAME		=	minishell

HEADER		=	includes/

LIBFT		=	libft/libft.a

CC			=	gcc

CFLAGS		=	-Werror -Wall -Wextra -g -I$(HEADER) -Ilibft -Llibft -lft -lreadline
LFLAGS		=	-Werror -Wall -Wextra -g
LIBFLAGS	=	-I$(HEADER) -Ilibft -Llibft -lft -lreadline

SRCS		=	srcs/minishell.c

OBJS		=	$(SRCS:.c=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJS) $(LIBFT) $(HEADER)
				$(CC) $(LFLAGS) $(OBJS) -o $(NAME) $(LIBFT) $(LIBFLAGS)

$(LIBFT)	:
				make gnl -C ./libft

clean		:
				rm -rf $(OBJS)
				make clean -C ./libft

fclean		:	clean
				rm $(NAME)
				make fclean -C ./libft

re			:	fclean all
