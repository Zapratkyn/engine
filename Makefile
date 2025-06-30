NAME					= myEngine
SRC						= src/* classes/*
GAME_SRC				= game_src/* game_classes/* 
LINKS					= -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CC						= g++
RM						= rm -rf
CFLAGS					= -std=c++20 -Wall -Wextra -Werror


all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LINKS)

game: $(OBJS)
	$(CC) $(CFLAGS) $(GAME_SRC) -o game $(LINKS)

clean:
			$(RM) $(OBJS)

fclean: clean
			$(RM) game

re: fclean game

.PHONY: all clean fclean re