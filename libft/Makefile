NAME 	= libft.a
SRCS 	= ft_atoi.c \
ft_bzero.c \
ft_calloc.c \
ft_palloc.c \
ft_itoa.c \
ft_split.c \
ft_substr.c \
ft_replacestr.c \
ft_atoilong.c \
char/ft_isalnum.c \
char/ft_isalpha.c \
char/ft_isascii.c \
char/ft_isdigit.c \
char/ft_isprint.c \
mem/ft_memchr.c \
mem/ft_memcmp.c \
mem/ft_memcpy.c \
mem/ft_memmove.c \
mem/ft_memset.c \
put/ft_putchar_fd.c \
put/ft_putendl_fd.c \
put/ft_putnbr_fd.c \
put/ft_putstr_fd.c \
str/ft_strchr.c \
str/ft_strdup.c \
str/ft_strdup_until.c \
str/ft_striteri.c \
str/ft_strjoin.c \
str/ft_strsjoin.c \
str/ft_strlcat.c \
str/ft_strlcpy.c \
str/ft_strlen.c \
str/ft_strmapi.c \
str/ft_strncmp.c \
str/ft_strnstr.c \
str/ft_strrchr.c \
str/ft_strtrim.c \
str/ft_strcmp.c \
str/ft_stris.c \
char/ft_tolower.c \
char/ft_toupper.c
SRCS_B 	= list/ft_lstadd_back.c \
list/ft_lstadd_front.c \
list/ft_lstclear.c \
list/ft_lstdelone.c \
list/ft_lstiter.c \
list/ft_lstlast.c \
list/ft_lstmap.c \
list/ft_lstnew.c \
list/ft_lstsize.c 

GNL 	= gnl/get_next_line.c \
gnl/get_next_line_utils.c 

OBJS 	= ${SRCS:.c=.o}
OBJS_B	= ${SRCS_B:.c=.o}
GNL_O	= ${GNL:.c=.o}


CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

.c.o:
	@echo Compiling.. $<
	@${CC} ${CFLAGS} -I. -Ignl -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
	@echo Creating library.. ${NAME}
	ar rcs $@ $^

all: ${NAME}
	@echo Done.

bonus: ${OBJS} ${OBJS_B} 
	@echo Bonus..
	@ar rcs ${NAME} $^

gnl: ${GNL_O} bonus
	@echo $<
	@echo Libft with gnl..
	@ar rcs ${NAME} $<

clean:
	@echo Deleting.. ${OBJS}
	@${RM} ${OBJS}
	@echo Deleting.. ${OBJS_B}
	@${RM} ${OBJS_B}
	@echo Deleting.. ${GNL_O}
	@${RM} ${GNL_O}

fclean: clean
	@echo Deleting.. ${NAME}
	@${RM} ${NAME}
	
re: fclean all

.PHONY: all clean fclean re bonus
