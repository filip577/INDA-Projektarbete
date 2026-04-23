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
#include "../include/map.h"
#include "../include/input.h"

/*
Takes values from the player struct and updates the player position on the map using trigonometry
*/
void player_position_update(t_player *p, t_map *map, t_input *input){
    float rotation__speed = 2.0;
    float movement_speed = 3.0;
    float new_player_x = p->player_x;
    float new_player_y = p->player_y;
    if(input-> turning_left){
        p-> player_angle -= rotation__speed;
    }
    if(input-> turning_right){
        p-> player_angle += rotation__speed;
    }
    if(input-> walking_forwards){
        new_player_x += cos(p->player_angle) * movement_speed;
        new_player_y += sin(p->player_angle) * movement_speed;
    }
    if(input-> walking_backwards){
        new_player_x -= cos(p->player_angle) * movement_speed;
        new_player_y -= sin(p->player_angle) * movement_speed;
    }
    
    if(!is_wall(map, new_player_x, new_player_y)){
        p->player_x = new_player_x;
        p->player_y = new_player_y;
    }
}