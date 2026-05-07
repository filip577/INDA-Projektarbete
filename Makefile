NAME = game

SRC = src/main.c \
      src/map.c \
      src/input.c \
      src/player.c \
      src/map_load.c\
      src/game.c\
      src/render.c \
      src/raycast.c

CC = gcc
CFLAGS = -Wall -Wextra -Werror \
         -DGL_SILENCE_DEPRECATION \
         -g \
         -fsanitize=address \
         -fno-omit-frame-pointer

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS = -fsanitize=address -framework OpenGL -framework GLUT
else
	LDFLAGS = -lGL -lGLU -lglut -lm
endif

all:
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LDFLAGS)

clean:
	rm -f $(NAME)

re: clean all
