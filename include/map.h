#ifndef MAP_H //Include guard, "if MAP_H isn't defined, keep reading the file"
#define MAP_H // define the symbol MAP_H

/* Define a struct for the map
*/
typedef struct s_map
{
    int width; //Width of map
    int height; //Height of map
    char **grid; //Double pointer to map layout, represents a collection of rows where each row is a string
    int player_start_x; //Starting x-value
    int player_start_y; //Starting y-value

} t_map;


/* Functions */
t_map build_map(char **layout); // function that builds a map, input char **layout, output a complete t_map, saves the grid, counts dimension and finds player starting coordinates
void find_player_start(t_map *map); //takes a pointer to t_map, goes through the map to find 'P', returns nothing
char get_tile(t_map *map, int x, int y); //Returns a tile form a map, takes a map, gets x and y coordinates and returns a char, '1' for wall '0' for floor 'P' for player start etc.
int is_wall(t_map *map, int x, int y); //Answers if a tile is a wall or a floor, returns int because 1 is wall 0 is floor and can be used in if statements for C
int is_inside_map(t_map *map, int x, int y); //Protective function. Checks if koordinates are inside the maps bounds, important to avoid crashes, e.g x>= 0, x<map->width
void print_map(t_map *map); //Debug function, can write out the map in terminal to check that everything is correct.

#endif

