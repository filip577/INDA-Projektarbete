#ifndef PLAYER_H //Prevents defining player_h twice
#define PLAYER_H // Define PLAYER_H

#include "../include/map.h"
#include "../include/input.h"

typedef struct s_enemy t_enemy;

/*
Defines a struct for the player
*/
typedef struct s_player {
    float player_x;
    float player_y;
    float player_angle;
    float rotation_speed;
    float movement_speed;
    float player_health;
} t_player;

void player_position_update(t_player *p, t_map *map, t_input *input); //Updates the player position on the map

void player_shoot(t_player *p, t_map *m, t_enemy *enemies[], int enemy_count, t_input *input); //Player shoots in the direction its facing

#endif
