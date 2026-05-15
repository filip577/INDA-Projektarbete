#ifndef GAME_H //Prevents defining game_h twice
#define GAME_H // Define GAME_H
#include "../include/map.h"    //Interface
#include "../include/player.h" // -II-
#include "../include/enemies.h"
#include "../include/sprite.h"
#include <stdlib.h>
#include <stdbool.h>


/**
 * High-level game state. Only GAME_PLAYING runs simulation; the rest
 * are end-states that show an overlay and wait for ESC.
 */
typedef enum e_game_state {
    GAME_PLAYING,
    GAME_WON,
    GAME_LOST
} t_game_state;

/**
 * Define a struct for the game
 */

 typedef struct s_game
 {
    t_map map;
    t_player *player;
    t_input *input;

    t_enemy enemies[MAX_ENEMIES];
    int enemy_count;
    t_enemy_sprites enemy_sprites; //GL textures + loaded flag, populated lazily on first render

    bool has_key;       //true once the player has picked up the key
    int prev_time_ms;   //last frame timestamp in ms, used to compute dt for enemy timers
    t_game_state state; //PLAYING / WON / LOST - drives overlay + simulation pause

    bool running;
 } t_game;


void init_game(t_game *game, const char *map_file); //Initialize the game, pointer to game object and search path to a map file
void init_player_from_map(t_game *game); //places the player with regards to game.map.player_start_x/y
void update_game(t_game *game); //Updates logic for each frame, e.g movement, cooldown, enemies, collectibles
void cleanup_game(t_game *game); //Frees memory before closing program



#endif
