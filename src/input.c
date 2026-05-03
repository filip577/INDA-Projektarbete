#include "../include/input.h"
#include <stdbool.h>
#include "../include/player.h"
#include "../include/enemies.h"
#include "../include/map.h"

/*
Updates the bool when one of the movement keys are pressed.
*/
void keyDown(unsigned char key, int x, int y) {
    if(key == 'a') input.turning_left = true;
    if(key == 'd') input.turning_right = true;
    if(key == 'w') input.walking_forwards = true;
    if(key == 's') input.walking_backwards = true;   
    if(key == ' ') input.shooting = true;  
}

/*
Updates the bool when one of the movement keys are released
*/
void keyUp(unsigned char key, int x, int y) {
    if(key == 'a') input.turning_left = false;
    if(key == 'd') input.turning_right = false;
    if(key == 'w') input.walking_forwards = false;
    if(key == 's') input.walking_backwards = false; 
}
