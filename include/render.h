#ifndef RENDER_H //Prevents defining RENDER_H twice
#define RENDER_H  // Define RENDER_H

#include "../include/game.h"

// Render one frame for the current game state.
void render_frame(t_game game);

// Render the world using raycasting.
void raycast_render(t_gamegame);

// Handle any renderer-specific input state or callbacks.
void handle_input(t_game *game);

void render_frame(t_game *game); //Per-frame render entry point called from the main loop

#endif
