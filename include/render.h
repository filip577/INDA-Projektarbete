#ifdef RENDER_H //Prevents defining RENDER_H twice
#define RENDER_H  // Define RENDER_H


#include #../include/game.h


void raycast_render(t_game *game); //Renders the 3D view of the game using raycasting

void display(t_game *game); //Displays the rendered view on the screen

#endif
