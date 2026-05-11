#ifndef ENEMIES_H //Prevents defining enemies_h twice
#define ENEMIES_H // Define enemies_h

#include <stdbool.h>
typedef struct s_enemy{
    bool alive;
    float enemy_x;
    float enemy_y;    
    int health;
    int amount_of_updates_since_seen_player;
    float last_seen_player_x;
    float last_seen_player_y;
    float enemy_wandering_angle;
    int enemy_wandering_updates;
}t_enemy;

void enemy_update_position(t_player *player, t_enemy *enemies[],t_map *map, int enemy_count); //Updates the enemy position depending on what state the enemy is in

bool player_visible_too_enemy(t_player *player, t_enemy *enemy, t_map *map, float player_direction); //Determines if the enemy can se the player from it's current position

#endif