#ifndef ENEMIES_H //Prevents defining enemies_h twice
#define ENEMIES_H // Define enemies_h

#include <stdbool.h>
typedef struct s_enemy{
    bool alive;
    float enemy_x;
    float enemy_y;    
    int health;
}t_enemy;

#endif