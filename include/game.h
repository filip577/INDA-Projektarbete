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
     t_map *map;
     t_player *player;

     bool running;
 } t_game;

 



#endif
