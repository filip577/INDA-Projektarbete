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
        ray_x += ray_cos * step_size; // Move the ray forward in small steps
        ray_y += ray_sin * step_size;
        distance += step_size;

        if (is_wall(map, (int)ray_x, (int)ray_y)) 
        {
            return distance; // Return the distance to the wall
        }
}


static void draw_wall_slice(int x, float distance)
{
    float wall_height;
    int half_screen_height = screen_height / 2;

    wall_height = (screen_height / distance) * 0.5f; // Calculate wall height based on distance (closer walls appear taller)

    if (wall_height > screen_height) {
        wall_height = screen_height; // Cap wall height to screen height
    }

    float
}