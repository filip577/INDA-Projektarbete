#include <stdio.h>
#include <stdlib.h>
#include <math.h>
    #ifdef __APPLE__
        #include <GLUT/glut.h>
        #include <OpenGL/gl.h>
    #else
        #include <GL/glut.h>
        #include <GL/gl.h>
        
    #endif
#include "../include/player.h"
#include <stdbool.h>
#include "../include/map.h"'

#define M_PI 3.14159265358979323846

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
void player_position_update(t_player *p, t_map *map){
    float new_player_x = p->player_x;
    float new_player_y = p->player_y;
    if(p-> player_angle < 0 || p-> player_angle > 2 * M_PI){
        p->player_angle = fmod(p->player_angle, 2 * M_PI);
    }
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
    if(is_wall(map, new_player_x, new_player_y) && ((p->player_angle < M_PI/2 && p->player_angle > M_PI / 4) 
    || (p->player_angle > M_PI * 3 / 2 && p->player_angle < 7 / 4 * M_PI))){
        p->player_x += cos(p->player_angle) * movement_speed;
    }
    if(is_wall(map, new_player_x, new_player_y) && ((p->player_angle > M_PI/2 && p->player_angle > M_PI / 4 * 3) 
    || (p->player_angle < M_PI * 3 / 2 && p->player_angle > 5 / 4 * M_PI))){
        p->player_x -= cos(p->player_angle) * movement_speed;
    }
    if(is_wall(map, new_player_x, new_player_y) && ((p->player_angle > 0 && p->player_angle < M_PI / 4) 
    || (p->player_angle < M_PI && p->player_angle > 3 / 4 * M_PI))){
        p->player_x += sin(p->player_angle) * movement_speed;
    }
    if(is_wall(map, new_player_x, new_player_y) && ((p->player_angle < 2 * M_PI && p->player_angle > M_PI * 7 / 4) 
    || (p->player_angle > M_PI && p->player_angle < 5 / 4 * M_PI))){
        p->player_x -= sin(p->player_angle) * movement_speed;
    }
}