NAME = minishell

SRCS = 	minishell.c\
		utils.c\
		signals.c\
		quotes.c\
		
CC = gcc
CFLAGS = -Wall -Wextra -Werror -lreadline -lhistory -lncurses

OBJS = $(SRCS:.c=.o)
RM = rm -f

all: ${NAME}
${NAME}: ${OBJS}
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean: 
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re