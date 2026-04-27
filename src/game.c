#include <stdio.h>
#include "../include/game.h"
#include "../include/input.h"
#include "../include/render.h"



/**
 * Method to connect map to player system, places player in middle of grid tile if tile length is 1
 */
void init_player_from_map(t_game *game)
{
    game->player.player_x = game->map.player_start_x + 0.5;
    game->player.player_y = game->map.player_start_y + 0.5;
    game->player.player_angle = 0.0; //starting angle
    game->player.player_move_speed = 0.1; //starting rotation and movement speed
    game->player.player_rot_speed = 0.1;
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
    (void)game;
}


/**
 * Main game loop function
 */
void game_loop(t_game *game)
{
    while(game->running)
    {
        handle_input(game); //reads keyboard/mouse input
        update_game(game); //updates the logic
        render_frame(game); //renders the frame
    }
}


/**
 * Cleans up game, frees map from memory etc.
 */
void cleanup_game(t_game *game)
{
    free_map(&game->map);
}




