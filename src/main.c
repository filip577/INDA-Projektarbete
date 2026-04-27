#include "../include/game.h"
#include <stdio.h>

static const char *choose_map(void)
{
    int choice;

    printf("Choose a map:\n");
    printf("1. level1\n");
	printf("2. level2\n");
	printf("3. level3\n");
    printf("4. ai generated map\n");
	printf("Enter choice: ");
	scanf("%d", &choice);


    if (choice == 2)
		return ("assets/maps/level2.txt");
	if (choice == 3)
		return ("assets/maps/level3.txt");
    if (choice == 4)
        system("python3 tools/generate_maps.py"); //TODO
	return ("assets/maps/level1.txt");

    printf("Invalid choice. Defaulting to level1.\n");
	return ("assets/maps/level1.txt");

}

int main(void)
{
    t_game game;
    const char *map_file;

    map_file = choose_map();
    init_game(&game, map_file);
    game_loop(&game);
    cleanup(&game);
    return (0);

}
