NAME					= engine
SRC						= main.cpp glad.c Core/* Graphics/* Input/*
LINKS					= -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CC						= g++
RM						= rm -rf
CFLAGS					= -std=c++20 -Wall -Wextra -Werror


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LINKS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) engine

re: fclean all

.PHONY: all clean fclean re