#ifndef MAP_H //Include guard, "if MAP_H isn't defined, keep reading the file"
#define MAP_H // define the symbol MAP_H

/* Maximum number of enemy spawn points a single map may declare */
#define MAX_ENEMIES 32

/*
Map tile legend:
    '1' = wall
    '0' = floor
    'P' = player start (exactly one)
    'K' = key tile      (exactly one)
    'D' = exit/door     (exactly one)
    'E' = enemy spawn   (one or more)
All tiles other than '1' are walkable.
*/

/* Define a struct for the map
*/
typedef struct s_map
{
    int width;  //Width of map
    int height; //Height of map
    char **grid; //Double pointer to map layout, represents a collection of rows where each row is a string

    int player_start_x; //Starting x-value
    int player_start_y; //Starting y-value

    int key_x;          //Tile coordinate of the key (-1 if not yet found)
    int key_y;
    int exit_x;         //Tile coordinate of the exit door
    int exit_y;

    int enemy_spawn_x[MAX_ENEMIES]; //Parallel arrays for the 'E' tile coordinates
    int enemy_spawn_y[MAX_ENEMIES];
    int enemy_spawn_count;          //How many enemy spawn tiles were found
} t_map;


/* Functions */

/* Map creation */
t_map load_map_from_file(const char *filename); // Method to load/create a map from a file, takes in a filename and returns a complete t_map

/* Map parsing / setup */
void find_player_start(t_map *map); //takes a pointer to t_map, goes through the map to find 'P', returns nothing
void find_special_tiles(t_map *map); //records key, exit and enemy spawn coordinates in the map struct

/* Map queries */
char get_tile(t_map *map, int x, int y); //Returns a tile form a map, takes a map, gets x and y coordinates and returns a char, '1' for wall '0' for floor 'P' for player start etc.
int is_wall(t_map *map, int x, int y); //Answers if a tile is a wall or a floor, returns int because 1 is wall 0 is floor and can be used in if statements for C
int is_inside_map(t_map *map, int x, int y); //Protective function. Checks if koordinates are inside the maps bounds, important to avoid crashes, e.g x>= 0, x<map->width

/* Debug */
void print_map(t_map *map); //Debug function, can write out the map in terminal to check that everything is correct.

/* Memory */
void free_map(t_map *map); //Method to release memory after map is built

#endif
