#include "GL/glut.h"
#include <math.h>
#include "../include/game.h"
#include "../include/player.h"
#include "../include/game.h"

#define FOV 60.0 // Field of view in degrees
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_DEPTH 20.0 // Rendering distance of 20

/**
 * Cast a ray from the player's position at a given angle and return the distance to the nearest wall or max reindering distance if no wall is hit.
 */

static float cast_ray(t_player *player, t_map *map, float ray_angle) {
    float ray_x = player->player_x;
    float ray_y = player->player_y;
    float ray_cos = cos(ray_angle);
    float ray_sin = sin(ray_angle);
    float distance = 0.0f;
    float step_size = 0.1f; // Step size for ray marching

    while (distance < max_render_distance) {
        // Move the ray forward in small steps
        ray_x += ray_cos * step_size; 
        ray_y += ray_sin * step_size;
        distance += step_size;

        if (is_wall(map, (int)ray_x, (int)ray_y)) 
        {
            // Return the distance to the wall
            return distance; 
        }
}

static float degrees_to_radians(float degrees) 
{
    return degrees * (M_PI / 180.0f); 
}

/**
 * Draw a vertical slice of the wall at the given screen x coordinate based on the distance to the wall. Closer walls appear taller and brighter, while farther walls appear shorter and darker. type shit.
 */
static void draw_wall_slice(int x, float distance)
{
    float wall_height;
    int half_screen_height = screen_height / 2;

    // Calculate wall height based on distance (closer walls appear taller)
    wall_height = (screen_height / distance) * 0.5f; 

    if (wall_height > screen_height) {

        // Cap wall height to screen height
        wall_height = screen_height; 
    }

    // Calculate starting y position to center the wall slice
    float start_y = (screen_height - wall_height) / 2.0f; 
    // Calculate ending y position of the wall slice
    float end_y = start_y + wall_height; 

    // Calculate brightness based on distance (farther walls are darker)
    float brightness = 1.0f - (distance / max_render_distance) *0.67f; 
    // Set wall color based on distance
    glColor3f(brightness, brightness, brightness); 

    // Draw a vertical slice of the wall as a quad a.k.a. a four sided polygon or a rectangle 
    glBegin(GL_QUADS); 
    // Top left corner of the wall slice
    glVertex2f(x, start_y); 
    // Top right corner of the wall slice
    glVertex2f(x + 1, start_y); 
    // Bottom right corner of the wall slice
    glVertex2f(x + 1, end_y); 
    // Bottom left corner of the wall slice
    glVertex2f(x, end_y); 
    // Finish drawing the wall slice
    glEnd(); 

}

void raycast_render(t_game *game) 
{
    int x;
    float angle_offset;
    float ray_angle;
    float distance;
    float fov_radians;
    float half_fov;

    if (!game || !game->player || !game->map) {
        // Ensure game, player, and map are valid before rendering
        return; 
    }

    // Convert field of view from degrees to radians
    fov_radians = degrees_to_radians(FOV); 
    half_fov = fov_radians / 2.0f;

    for (x = 0; x < screen_width; x++)
    {
        // Calculate angle offset for the current column centered around the player's view
        angle_offset = (float)x / screen_width - 0.5f; 
        // Calculate the angle of the ray for the current column
        ray_angle = game->player->player_angle + (angle_offset * fov_radians); 
        // Cast the ray and get the distance to the nearest wall
        distance = cast_ray(game->player, game->map, ray_angle); 
        // Correct distance for fish-eye effect
        distance *= cos(angle_offset * fov_radians); 
        // Draw the vertical slice of the wall based on the distance
        draw_wall_slice(x, distance); 

    }

    void display(t_game *game)
{
    // Clear the screen and depth buffer
    glClear(gl_color_buffer_bit | gl_depth_buffer_bit); 

    // Set up the projection matrix for 2D rendering
    glMatrixMode(gl_projection);
    glLoadIdentity();
    gluOrtho2D(0, screen_width, 0, screen_height, 0,0f, -1.0f, 1.0f);
    glMatrixMode(gl_modelview);
    glPushMatrix();
    glLoadIdentity();

    // Render the "3D" raycasted view of the game
    raycast_render(game);

    // Restore matrices
    glPopMatrix();
    glMatrixMode(gl_projection);
    glLoadIdentity();
    glMatrixMode(gl_modelview);

    // Swap buffers to display the rendered frame
    glutSwapBuffers();

}

