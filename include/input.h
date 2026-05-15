#ifndef INPUT_H  //Prevents defining input_h twice
#define INPUT_H // Define input_h
#include <stdbool.h>
struct s_game;

typedef struct s_input{
    bool turning_left;
    bool turning_right;
    bool walking_forwards;
    bool walking_backwards;
    bool shooting;       //true while space is held; cooldown gates fire rate
    bool pickup_pressed; //one-shot, set on F keydown
    bool quit_pressed;   //one-shot, set on ESC keydown
}t_input;

extern t_input input; //creates a variable of the struct used for the input handling and update the player

void keyDown(unsigned char key, int x, int y); //A func that which updates a bool or calls another func depending on which key has been pressed

void keyUp(unsigned char key, int x, int y);  //A func that updates a bool when some keys are released

void handle_input(struct s_game *game); //handles input from the player

#endif
