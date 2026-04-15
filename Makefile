NAME = game

SRC = src/main.c \
      src/map.c \
      src/player.c \
      src/render.c \
      src/raycast.c

CC = gcc
CFLAGS = -Wall -Wextra -Werror

all:
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all
