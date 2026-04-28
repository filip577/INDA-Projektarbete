#ifndef PLAYER_H //Prevents defining player_h twice
#define PLAYER_H // Define PLAYER_H

#include "../include/map.h"
#include "../include/input.h"

/*
Defines a struct for the player
*/
typedef struct s_player {
    float player_x;
    float player_y;
    float player_angle;
    float rotation_speed;
    float movement_speed;
} t_player;

void player_position_update(t_player *p, t_map *map, t_input *input); //Updates the player position on the map

#endif
