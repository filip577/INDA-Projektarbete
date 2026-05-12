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
#include "../include/enemies.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
Takes values from the player struct and updates the player position on the map using trigonometry
*/
void player_position_update(t_player *p, t_map *map, t_input *input){
    float new_player_x = p->player_x;
    float new_player_y = p->player_y;
    if(input-> turning_left){
        p-> player_angle -= p->rotation_speed;
    }
    if(input-> turning_right){
        p-> player_angle += p->rotation_speed;
    }
    if(input-> walking_forwards){
        new_player_x += cos(p->player_angle) * p->movement_speed;
        new_player_y += sin(p->player_angle) * p->movement_speed;
    }
    if(input-> walking_backwards){
        new_player_x -= cos(p->player_angle) * p->movement_speed;
        new_player_y -= sin(p->player_angle) * p->movement_speed;
    }
    
    //Updates player position if new position isn't wall
    if(!is_wall(map, new_player_x, new_player_y)){
        p->player_x = new_player_x;
        p->player_y = new_player_y;
    }
    //If player walks into a wall it only updates one of the coordinates depending on which way the wall is
    else{
        if(!is_wall(map, p->player_x, new_player_y)){
        p->player_y = new_player_y;
        } 
        if(!is_wall(map, new_player_x, p->player_y)){
        p->player_x = new_player_x;
        }  
    }
}
/*
*shoots a ray in the dircetion the player is looking and check the distance between the ray and all enemies and 
if it collides it does damage to the enemy
*/
void player_shoot(t_player *p, t_map *m, t_enemy *enemies[], int enemy_count, t_input *input){
    input-> shooting = false;
    float shooting_ray_x = p-> player_x;
    float shooting_ray_y = p-> player_y;
    float ray_step_distance = 0.05f;
    while(!is_wall(m, shooting_ray_x, shooting_ray_y)){
        shooting_ray_x += cos(p-> player_angle) * ray_step_distance;
        shooting_ray_y += sin(p-> player_angle) * ray_step_distance;
        for(int i = 0; i < enemy_count; i++){
            if(enemies[i]->alive){
                float distance = sqrt(pow(shooting_ray_x - enemies[i]->enemy_x, 2) + pow(shooting_ray_y - enemies[i]->enemy_y,2));
                if(distance < 0.01){
                    enemies[i]->health -= 25 - 0.1 * sqrt(pow(p-> player_x - enemies[i]->enemy_x, 2) + 
                    pow(p-> player_y - enemies[i]->enemy_y,2)); //Damage drop off based on distance from player to enemy
                    if(enemies[i]->health <= 0){
                        enemies[i] = false;
                        enemy_count--;
                    }
                    return;
                }
            }
        }

    }
}
