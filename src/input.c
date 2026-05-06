#include "../include/input.h"
#include "../include/game.h"
#include "../include/player.h"
#include <stdbool.h>
#include "../include/player.h"
#include "../include/enemies.h"
#include "../include/map.h"

t_input input;

/*
Updates the bool when one of the movement keys are pressed.
*/
void keyDown(unsigned char key, int x, int y) {
    (void)x;
    (void)y; //Silences warning about unused variable as y and x are only needed for mouse inputs which aren't needed
    if(key == 'a') input.turning_left = true;
    if(key == 'd') input.turning_right = true;
    if(key == 'w') input.walking_forwards = true;
    if(key == 's') input.walking_backwards = true;   
    //if(key == ' ') input.shooting = true;  
}

/*
Updates the bool when one of the movement keys are released
*/
void keyUp(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    if(key == 'a') input.turning_left = false;
    if(key == 'd') input.turning_right = false;
    if(key == 'w') input.walking_forwards = false;
    if(key == 's') input.walking_backwards = false; 
}



void handle_input(struct s_game *game) {
	player_position_update(game->player, &game->map, game->input);
}
