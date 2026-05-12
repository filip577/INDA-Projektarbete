#include "../include/enemies.h"
#include "../include/player.h"
#include "../include/map.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

t_enemy enemies[10];
int enemy_count = 1;

void enemy_update_position(t_player *player, t_enemy *enemies[],t_map *map, int enemy_count){
    float movement_speed = 1.5;
    for(int i = 0; i < enemy_count; i++){
        //atan2() handles division with zero which atan() doesn't safely do
        float player_direction = atan2(player->player_y - enemies[i]->enemy_y , player->player_x - enemies[i]->enemy_x); 
        if(player_visible_too_enemy(player, enemies[i], map, player_direction)){
            enemies[i]->amount_of_updates_since_seen_player = 0;
            enemies[i]->enemy_x += cos(player_direction) * movement_speed;
            enemies[i]->enemy_y += sin(player_direction) * movement_speed;
            float distance = sqrt(pow(player->player_x - enemies[i]->enemy_x, 2) + pow(player->player_y - enemies[i]->enemy_y, 2));
            if(distance < 0.01){
                player-> player_health -= 25;
            }         
        }
        if(!player_visible_too_enemy(player, enemies[i], map, player_direction) && (enemies[i]->amount_of_updates_since_seen_player = 0)){
            enemies[i]->amount_of_updates_since_seen_player = 1;
        }
        if(!player_visible_too_enemy(player, enemies[i], map, player_direction) && ((enemies[i]->amount_of_updates_since_seen_player >= 1) && (enemies[i]->amount_of_updates_since_seen_player <= 10))){
            enemies[i]->amount_of_updates_since_seen_player += 1;
            float angle_to_last_seen_position = atan2(enemies[i]->last_seen_player_y - enemies[i]->enemy_y , enemies[i]->last_seen_player_x - enemies[i]->enemy_x);
            enemies[i]->enemy_x += cos(angle_to_last_seen_position) * movement_speed;
            enemies[i]->enemy_y += sin(angle_to_last_seen_position) * movement_speed;
        }
        if(!player_visible_too_enemy(player, enemies[i], map, player_direction) && (enemies[i]->amount_of_updates_since_seen_player >= 11)){
            if(enemies[i]->enemy_wandering_updates > 30){
                enemies[i]->enemy_wandering_angle = ((float)(rand() % 100) / 100 ) * 2.0 * M_PI;
                enemies[i]->enemy_wandering_updates = 0;
            }
            float new_enemy_x = enemies[i]->enemy_x + cos(enemies[i]->enemy_wandering_angle) * movement_speed;
            float new_enemy_y = enemies[i]->enemy_y + sin(enemies[i]->enemy_wandering_angle) * movement_speed;
            if(!is_wall(map, new_enemy_x, new_enemy_y)){
                enemies[i]->enemy_x = new_enemy_x;
                enemies[i]->enemy_y = new_enemy_y;
            }
            enemies[i]->enemy_wandering_updates++;
        }
    }
}

bool player_visible_too_enemy(t_player *player, t_enemy *enemy, t_map *map, float player_direction){
    float enemy_vision_ray_x = enemy->enemy_x;
    float enemy_vision_ray_y = enemy->enemy_y;
    float ray_step_distance = 0.05f;
    while (!is_wall(map, enemy_vision_ray_x, enemy_vision_ray_y)){
        enemy_vision_ray_x += cos(player_direction) * ray_step_distance;
        enemy_vision_ray_y += sin(player_direction) * ray_step_distance;
        float distance = sqrt(pow(player->player_x - enemy_vision_ray_x, 2) + pow(player->player_y - enemy_vision_ray_y, 2));
        if(distance < 0.01){
            return true;
        }
    }
    return false;
}


