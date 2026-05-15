#include "../include/input.h"
#include "../include/game.h"
#include "../include/player.h"
#include <stdbool.h>
#include "../include/player.h"
#include "../include/enemies.h"
#include "../include/map.h"

t_input input;

/*
ASCII value of the Escape key. GLUT's keyDown gives us raw unsigned chars.
*/
#define KEY_ESCAPE 27

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
    if(key == 'f') input.pickup_pressed = true;   //one-shot, consumed by update_game
    if(key == ' ') input.shooting = true;          //held while pressed; cooldown gates fire rate
    if(key == KEY_ESCAPE) input.quit_pressed = true;
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
    if(key == ' ') input.shooting = false;
}



void handle_input(struct s_game *game) {
    /* Freeze player movement on the win/lose screen so the only thing left
       is to press ESC and exit. update_game still runs (it picks up the
       quit_pressed flag and ends the loop), but the world is paused. */
    if (game->state != GAME_PLAYING)
        return;
    player_position_update(game->player, &game->map, game->input);
}
