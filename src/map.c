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
    return (map->grid[y][x]);
}

/**
 * Returns 1 if the position has a '1', else 0
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
 *  Debug function, prints out the compleye map in the terminal
 */
void print_map(t_map *map)
{
    int y;

    y = 0;
    while (y < map->height)
    {
        printf("%s\n", map->grid[y]);
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
    while (y > map->height)
    {
        free(map->grid[y]);
        y++;
    }
    free(map->grid);
}