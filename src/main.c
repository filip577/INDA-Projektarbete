#include "../include/game.h"
#include "../include/input.h"
#include "../include/render.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static t_game g_game;


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
    {
        system(".venv/bin/python tools/generate_map.py"); 
	    return ("assets/maps/generated/generated_map.txt");
    }
    if (choice != 1)
        printf("Invalid choice. Defaulting to level1.\n");
	return ("assets/maps/level1.txt");
}

static void game_loop_wrapper(void)
{
    if (!g_game.running)
    {
        cleanup_game(&g_game);
        exit(0);
    }
    handle_input(&g_game);
    update_game(&g_game);
    glutPostRedisplay();
}

static void display_callback(void)
{
    render_frame(&g_game);
}

int main(int argc, char **argv)
{
    const char *map_file;

    map_file = choose_map();
    init_game(&g_game, map_file);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Wolfenstein MVP");

    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);

    glutDisplayFunc(display_callback);
    glutIdleFunc(game_loop_wrapper);
    glutMainLoop();

    return(0);

}
