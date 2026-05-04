#ifndef RENDER_H
#define RENDER_H

#include "../include/game.h"

// Initialize the renderer. This sets up GLUT and OpenGL state.
void init_renderer(void);

// Render one frame for the current game state.
void render_frame(t_game game);

// Render the world using raycasting.
void raycast_render(t_gamegame);

// Handle any renderer-specific input state or callbacks.
void handle_input(t_game *game);

#endif