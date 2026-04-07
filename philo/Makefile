NAME	= philo

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	= main.c \
		  utils.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

san:
	$(CC) $(CFLAGS) -fsanitize=thread -g $(SRCS) -lpthread -o $(NAME)_san

.PHONY: all clean fclean re san
