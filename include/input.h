#ifndef INPUT_H  //Prevents defining input_h twice
#define INPUT_H // Define input_h
#include <stdbool.h>
struct s_game;

typedef struct s_input{
    bool turning_left;
    bool turning_right;
    bool walking_forwards;
    bool walking_backwards;
}t_input;

extern t_input input; //creates a variable of the struct used for the input handling and update the player

void keyDown(unsigned char key, int x, int y); //A opengl func which checks if a key is pressed down and exectues an action

void keyUp(unsigned char key, int x, int y);  //A opengl func which checks if a key is released and then updates a bool

void handle_input(struct s_game *game); //handles input from the player

#endif