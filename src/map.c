#include <stdio.h> //Library for i/o
#include <stdlib.h> //Library for memory allocation
#include "../include/map.h"    //Interface

/**
 * Takes a pointer to a map, takes coordinates x,y and returns an int. Returns 1 if coordinate is inside map, 0 if outside
 */
int is_inside_map(t_map *map, int x, int y)
{
    return (x >=0 && x < map->width && y >= 0 && y < map->height);
}

/**
 * Takes map, x and y, returns the character in that specific position, returns '1' (wall) if out of bounds
 */
char get_tile(t_map *map, int x, int y)
{
    if (!is_inside_map(map, x, y))
        return ('1');
    return (map->grid[y][x]); //if the position is valid we return the contents for the position
}

/**
 * Returns 1 if the position has a '1', else 0.
 * All other tile types (P, K, D, E, 0) are walkable.
 */
int is_wall(t_map *map, int x,int y)
{
    return (get_tile(map, x, y) == '1');
}

/**
 * Finds the players starting position in the map, returns nothing, but updates map at start of game
 */
void find_player_start(t_map *map)
{
    //Loop variables to iterate through the map
    int x;
    int y;

    y = 0;
    while (y < map->height)
    {
            x = 0;
            while (x < map->width)
            {
                if (map->grid[y][x] == 'P')
                {
                        map->player_start_x = x;
                        map->player_start_y = y;
                        return;
                }
                x++;
            }
            y++;
    }
}

/**
 * Single sweep across the grid that records the key tile, exit tile and
 * every enemy spawn tile. Assumes validation already enforced exactly one
 * 'K', one 'D' and at least one 'E'.
 */
void find_special_tiles(t_map *map)
{
    int x;
    int y;

    map->key_x = -1;
    map->key_y = -1;
    map->exit_x = -1;
    map->exit_y = -1;
    map->enemy_spawn_count = 0;

    y = 0;
    while (y < map->height)
    {
        x = 0;
        while (x < map->width)
        {
            char tile = map->grid[y][x];
            if (tile == 'K')
            {
                map->key_x = x;
                map->key_y = y;
            }
            else if (tile == 'D')
            {
                map->exit_x = x;
                map->exit_y = y;
            }
            else if (tile == 'E' && map->enemy_spawn_count < MAX_ENEMIES)
            {
                map->enemy_spawn_x[map->enemy_spawn_count] = x;
                map->enemy_spawn_y[map->enemy_spawn_count] = y;
                map->enemy_spawn_count++;
            }
            x++;
        }
        y++;
    }
}

/**
 *  Debug function, prints out the compleye map in the terminal
 */
void print_map(t_map *map)
{
    int y;

    y = 0;
    while (y < map->height)
    {
        printf("%s\n", map->grid[y]); //Iterates through each line, prints out the whole line as a string
        y++;
    }
}

/**
 * Releases memory which is used by the map
 */
void free_map(t_map *map)
{
    int y;

    y = 0;
    while (y < map->height)
    {
        free(map->grid[y]); //Free every line separately, since every line was allocated separatley
        y++;
    }
    free(map->grid); //Free the actual array which held the pointers to the lines
    map->grid = NULL;
}
