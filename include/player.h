#ifndef Player_H //Prevents defining player_h twice
#define Player_H // Define Player_H

#include "../include/map.h"

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

float rotation__speed = 2.0;
float movement_speed = 3.0;

void keyDown(unsigned char key, int x, int y); //Updates a boolean if a movement key is pressed

void keyUp(unsigned char key, int x, int y); //Updates a boolean if a movement key is released

void player_position_update(t_player *p, t_map *map); //Updates the player position on the map

void player_shoot(t_player *p, t_map *m, t_enemy *enemies[], int enemy_count); //Player shoots in the direction its facing

#endif
