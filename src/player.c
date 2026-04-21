#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "../include/player.h"
#include <stdbool.h>
#include "../include/map.h"

bool turning_left;
bool turning_right;
bool walking_forwards;
bool walking_backwards;

/*
Updates the bool when one of the movement keys are pressed.
*/
void keyDown(unsigned char key, int x, int y) {
    if(key == 'a') turning_left = true;
    if(key == 'd') turning_right = true;
    if(key == 'w') walking_forwards = true;
    if(key == 's') walking_backwards = true;    
}

/*
Updates the bool when one of the movement keys are released
*/
void keyUp(unsigned char key, int x, int y) {
    if(key == 'a') turning_left = false;
    if(key == 'd') turning_right = false;
    if(key == 'w') walking_forwards = false;
    if(key == 's') walking_backwards = false; 
}

/*
Takes values from the player struct and updates the player position on the map using trigonometry
*/
void player_position_update(Player *p, t_map *map){
    float rotation__speed = 2.0;
    float movement_speed = 3.0;
    float new_player_x = p->player_x;
    float new_player_y = p->player_y;
    if(turning_left){
        p-> player_angle -= rotation__speed;
    }
    if(turning_right){
        p-> player_angle += rotation__speed;
    }
    if(walking_forwards){
        new_player_x += cos(p->player_angle) * movement_speed;
        new_player_y += sin(p->player_angle) * movement_speed;
    }
    if(walking_backwards){
        new_player_x -= cos(p->player_angle) * movement_speed;
        new_player_y -= sin(p->player_angle) * movement_speed;
    }
    
    if(!is_wall(map, new_player_x, new_player_y)){
        p->player_x = new_player_x;
        p->player_y = new_player_y;
    }
}