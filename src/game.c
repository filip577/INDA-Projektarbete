#include <stdio.h>
#include "../include/game.h"
#include "../include/input.h"
#include "../include/render.h"



/**
 * Method to connect map to player system, places player in middle of grid tile if tile length is 1
 */
void init_player_from_map(t_game *game)
{
    game->player->player_x = game->map.player_start_x + 0.5;
    game->player->player_y = game->map.player_start_y + 0.5;
    game->player->player_angle = 0.0; //starting angle
    game->player->movement_speed = 0.1; //starting rotation and movement speed
    game->player->rotation_speed = 0.1;
}


/**
 * Method that initialises the game
 */
void init_game(t_game *game, const char *map_file)
{
    game->map = load_map_from_file(map_file); //loads map from file using map_load.c function
    init_player_from_map(game); //initialises the player
    game->running = true; //game can run when true
}

/**
 * Function that updates game, currently empty just for placeholding. Will be filled with collectibles, enemies, timers and win condition
 */
void update_game(t_game *game)
{
    int player_tile_x;
    int player_tile_y;
    char current_tile;

    player_tile_x = (int)game->player->player_x;
    player_tile_y = (int)game->player->player_y;

    current_tile = get_tile(&game->map, player_tile_x, player_tile_y);

    if (current_tile == 'D')
    {
        printf("You found the exit!\n");
        game->running = false;
    }
}


/**
 * Cleans up game, frees map from memory etc.
 */
void cleanup_game(t_game *game)
{
    free_map(&game->map);
}




