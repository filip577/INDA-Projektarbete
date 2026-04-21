#ifndef Player_H //Prevents defining player_h twice
#define Player_H // Define Player_H

#include "../include/map.h"

/* 
Defines a struct for the player
*/
typedef struct player {
    float player_x;
    float player_y;
    float player_angle;
} Player;


void keyDown(unsigned char key, int x, int y); //Updates a boolean if a movement key is pressed

void keyUp(unsigned char key, int x, int y); //Updates a boolean if a movement key is released

void player_position_update(Player *p, t_map *map); //Updates the player position on the map

#endif
