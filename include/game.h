#ifndef GAME_H //Prevents defining game_h twice
#define GAME_H // Define GAME_H
#include "../include/map.h"    //Interface
#include "../include/player.h" // -II-
#include <stdlib.h>


/**
 * Define a struct for the game
 */

 typedef struct s_game
 {
     t_map map;
     t_player player;

     bool running;
 } t_game;

 
void init_game(t_game *game, const char *map_file); //Initialize the game, pointer to game object and search path to a map file
void init_player_from_map(t_game *game); //places the player with regards to game.map.player_start_x/y
void update_game(t_game *game); //Updates logic for each frame, e.g movement, cooldown, enemies, collectibles
void game_loop(t_game *game); //main game loop while the game is running
void cleanup_game(t_game *game); //Frees memory before closing program



#endif
