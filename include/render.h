#ifndef RENDER_H //Prevents defining RENDER_H twice
#define RENDER_H  // Define RENDER_H

#include "../include/game.h"

// Render one frame for the current game state.
void render_frame(t_game *game);

// Render the world using raycasting.
void raycast_render(t_game *game);

// Handle any renderer-specific input state or callbacks.
void handle_input(t_game *game);

// Accessors exposed by raycast.c so sprite rendering can occlude
// against the same wall depths the raycaster computed for this frame.
const float *raycast_get_wall_distances(void);
int raycast_get_screen_width(void);
float raycast_get_fov_radians(void);

// Draw all live enemies as billboarded sprites with per-column wall
// occlusion. Must be called AFTER raycast_render so the wall-distance
// buffer is populated for the current frame.
void render_enemies(t_game *game);

#endif
