NAME = scheduler

CC = gcc 
CFLAGS = -Wextra -Werror -Wall

SRCS =	scheduler.c

OBJS = $(SRCS:.c=.o)

OUTPUT = a.txt

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME) $(OUTPUT)

re: fclean all

.PHONY: clean fclean all re
