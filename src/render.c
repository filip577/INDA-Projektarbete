#include <stdbool.h>
#include <math.h>
#include "../include/render.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
#else
    #include <GL/glut.h>
    #include <GL/gl.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define FOV_DEGREES 60.0f
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_DEPTH 20.0f
#define STEP_SIZE 0.05f

static bool renderer_initialized = false;

/**
 * init_renderer initializes GLUT and OpenGL state. This is called
 * the first time the renderer is requested.
 */
static void init_renderer(void)
{
    if (renderer_initialized)
        return;

    int argc = 1;
    char *argv[2] = {"game", NULL};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("INDA Projektarbete");

    glClearColor(0.15f, 0.20f, 0.30f, 1.0f);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    renderer_initialized = true;
}

/**
 * degrees_to_radians converts an angle in degrees to radians.
static float degrees_to_radians(float degrees)
{
    return degrees * (M_PI / 180.0f);
}
 */

/**
 * cast_ray marches a ray from the player position in a given direction.
 * It returns the distance to the first wall hit, or MAX_DEPTH when no wall
 * is encountered within range.
static float cast_ray(t_game *game, float ray_angle)
{
    float ray_x = game->player->player_x;
    float ray_y = game->player->player_y;
    float ray_cos = cosf(ray_angle);
    float ray_sin = sinf(ray_angle);
    float distance = 0.0f;

    while (distance < MAX_DEPTH)
    {
        ray_x += ray_cos * STEP_SIZE;
        ray_y += ray_sin * STEP_SIZE;
        distance += STEP_SIZE;

        if (is_wall(&game->map, (int)ray_x, (int)ray_y))
            return distance;
    }
    return MAX_DEPTH;
}
*/

/**
 * draw_wall_slice draws a vertical slice of the wall at screen column x.
 * The wall height and brightness are based on the ray distance.

static void draw_wall_slice(int x, float distance)
{
    if (distance <= 0.0f)
        return;

    float wall_height = (SCREEN_HEIGHT / distance) * 0.75f;
    if (wall_height > SCREEN_HEIGHT)
        wall_height = SCREEN_HEIGHT;

    float start_y = (SCREEN_HEIGHT - wall_height) * 0.5f;
    float end_y = start_y + wall_height;

    float brightness = 1.0f - (distance / MAX_DEPTH) * 0.65f;
    if (brightness < 0.15f)
        brightness = 0.15f;

    glColor3f(brightness, brightness, brightness);
    glBegin(GL_QUADS);
    glVertex2f((float)x, start_y);
    glVertex2f((float)x + 1.0f, start_y);
    glVertex2f((float)x + 1.0f, end_y);
    glVertex2f((float)x, end_y);
    glEnd();
}
*/


/**
 * render_frame clears the screen, sets up the projection, renders the scene,
 * and swaps the GLUT double buffer to display the frame.
 */
void render_frame(t_game *game)
{
    init_renderer();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    raycast_render(game);

    glutSwapBuffers();
    glFlush();
}

/**
 * handle_input is a stub placeholder for future input-driven rendering hooks.

void handle_input(t_game *game)
{
    (void)game;
}
 */
