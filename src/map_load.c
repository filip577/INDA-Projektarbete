#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/map.h"

/**
 * Map_load has functions which:
 *  -reads mapfile
 *  -builds grid
 *  -validates format
 *  -validates player
 *  -fills t_map
 */


/**
 * Intern helper function, writes an error message and exits the program with error status
 */
static void exit_with_error(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

/**
 * Helperfunction that takes a line from the file, remoes \n, allocates a new string, copies its contents and returns the pointer to the new linex
 */
static char *duplicate_line_without_newline(const char *line)
{
    size_t len; //variable for length of the string
    char *copy; //where to store the new copy of the line

    len = strlen(line); //counts the size of the read line
    if (len > 0 && line[len-1] == '\n') //if last character is newline, we reduce length by 1, i.e copying without \n
        len--;

    copy = malloc(sizeof(char) * (len + 1)); //allocate memory for the new line, +1 needed for the null-terminator: \0
       if (!copy)
        exit_with_error("malloc failed while copying map line");

    memcpy(copy, line, len); //copies exact len number of characters from the original string to the new one
    copy[len] = '\0'; //puts null-terminator on the end to make it a C-stirng
    return (copy);       
}

/**
 * Helper method to count how many lines the mapfile has, to know how big the grid is supposed to be
 */
static int count_lines_in_file(const char *filename)
{
    FILE *file; //pointer to file which represents the open file
    char buffer[1024]; //temporary buffer where every line gets read, everytime fgets runs this buffer gets filled with a line
    int count; //counter for #lines

    file  = fopen(filename, "r"); //opens the file in readmode, if there is no file or it cannot be opened it will return NULL
    if (!file)
        exit_with_error("could not open map file"); //Exit directly if it fails to open file

    count = 0; //start with 0 lines
    while (fgets(buffer, sizeof(buffer),file)) //reads the file line by line until fgets cannot continue, for each line count increases by 1
        count++;

    fclose(file); //Close file
    if (count == 0)
        exit_with_error("map file is empty");
    return count;
}

/**
 * Helper if errors occur while reading the map
 * If we already allocated lines we need to free these before exiting
 */
static void free_partial_grid(char **grid, int rows_filled)
{
    int i;

    i = 0;
    while (i < rows_filled) //Looping through all the lines that have been filled
    {
        free(grid[i]); //frees every line
        i++;
    }
    free(grid); //frees the array with row pointers
}

/**
 * Reads the map contents from the file and builds the grid, returns a char **
 */
static char **read_grid_from_file(const char *filename, int height) 
{
    FILE *file; //Pointer to file
    char buffer[1024]; //temporary buffer for a line
    char **grid; //Where to store the map
    int i; //Line counter

    file = fopen(filename, "r"); //Opens the file, first time we counted the rows, now we want to read the contents
    if (!file)
        exit_with_error("could not reopen map file");

    grid = malloc(sizeof(char *) * height); //Allocates space for the lines, if height = 4 there is space for 4 char *
    if (!grid)
    {
        fclose(file);
        exit_with_error("malloc failed while allocating map grid");
    }

    i = 0;
    while (i < height && fgets(buffer, sizeof(buffer), file)) //Reads every line from the file, for every row it puts the text in buffer, duplicates to heap, stores in grid[i]
    {
        grid[i] = duplicate_line_without_newline(buffer); //Make a real copy of line, without newline, important because buffer is temporary
        if (!grid[i])
        {
            fclose(file);
            free_partial_grid(grid, i);
            exit_with_error("failed to store map line");
        }
        i++;
    }
    fclose(file);

    if (i != height) //Extra security, if we for some reason did not read exactly as many lines as we though, something is wrong
    {
        free_partial_grid(grid, i);
        exit_with_error("map height mismatch while reading file");
    }

    return grid;
}

/**
 * Checks that all lines has the same length, which means that the map is rectangular
 */
static void validate_rectangular_map(t_map *map)
{
    int y;
    size_t row_length; //length of the current line

    y = 0;
    while (y < map->height)
    {
        row_length = strlen(map->grid[y]); //Gets the length of the line
        if ((int)row_length != map->width) //If line is not the same length as first line, map isn't rectangular
        {
            free_map(map); //Free the maps memory and exits program with error message
            exit_with_error("map is not rectangular");
        }
        y++;
    }
}

/**
 * Single pass over the grid counting every tile we care about for
 * validation. Keeps validation cheap and avoids walking the grid once
 * per tile type.
 */
static void count_special_tiles(t_map *map, int *player_count, int *key_count,
                                int *exit_count, int *enemy_count)
{
    int x;
    int y;
    char tile;

    *player_count = 0;
    *key_count = 0;
    *exit_count = 0;
    *enemy_count = 0;

    y = 0;
    while (y < map->height)
    {
        x = 0;
        while (x < map->width)
        {
            tile = map->grid[y][x];
            if (tile == 'P')
                (*player_count)++;
            else if (tile == 'K')
                (*key_count)++;
            else if (tile == 'D')
                (*exit_count)++;
            else if (tile == 'E')
                (*enemy_count)++;
            x++;
        }
        y++;
    }
}

/**
 * Checks that the map declares exactly one player start, exactly one key,
 * exactly one exit and at least one enemy spawn. The "at least one enemy"
 * rule is the only enemy-related validation requirement.
 */
static void validate_special_tiles(t_map *map)
{
    int player_count;
    int key_count;
    int exit_count;
    int enemy_count;

    count_special_tiles(map, &player_count, &key_count, &exit_count, &enemy_count);

    if (player_count != 1)
    {
        free_map(map);
        exit_with_error("map must contain exactly one player start (P)");
    }
    if (key_count != 1)
    {
        free_map(map);
        exit_with_error("map must contain exactly one key (K)");
    }
    if (exit_count != 1)
    {
        free_map(map);
        exit_with_error("map must contain exactly one exit (D)");
    }
    if (enemy_count < 1)
    {
        free_map(map);
        exit_with_error("map must contain at least one enemy spawn (E)");
    }
    if (enemy_count > MAX_ENEMIES)
    {
        free_map(map);
        exit_with_error("map declares too many enemy spawns (E)");
    }
}

/**
 * Main function which the rest of the project will use
 * Loads the whole map from a file and returns a complete t_map
 */
t_map load_map_from_file(const char *filename)
{
    t_map map; //Creates a local map variable that will be filled step by step

    //Initializing with 0 data instead of junk data, -1 for no valid position
    map.width = 0;
    map.height = 0;
    map.grid = NULL;
    map.player_start_x = -1;
    map.player_start_y = -1;
    map.key_x = -1;
    map.key_y = -1;
    map.exit_x = -1;
    map.exit_y = -1;
    map.enemy_spawn_count = 0;

    map.height = count_lines_in_file(filename); //Count how many lines the file has = height
    map.grid = read_grid_from_file(filename, map.height); //Read the actual map to memory
    map.width = strlen(map.grid[0]); //Set the width to the length of the first row, because we need it to be rectanuglar all rows are same length

    validate_rectangular_map(&map); //Checks that all lines has same length
    validate_special_tiles(&map);   //Checks player / key / exit / enemy counts
    find_player_start(&map);        //Store player spawn coordinates
    find_special_tiles(&map);       //Store key, exit and enemy spawn coordinates

    return map;
}