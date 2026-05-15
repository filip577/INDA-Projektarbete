#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "../include/render.h"
#include "../include/sprite.h"
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

    // int argc = 1;
    // char *argv[2] = {"game", NULL};

    // glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    // glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    // glutInitWindowPosition(100, 100);
    // glutCreateWindow("INDA Projektarbete");

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
 * Draws a single line of text on the screen using GLUT's bitmap font.
 */
static void draw_text(float x, float y, const char *text)
{
    const char *c;

    glRasterPos2f(x, y);
    for (c = text; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

/**
 * Draws a single line of larger text. Used for the win/lose headline.
 */
static void draw_text_big(float x, float y, const char *text)
{
    const char *c;

    glRasterPos2f(x, y);
    for (c = text; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
}

/**
 * Compute the approximate width in pixels of a Helvetica-18 string so
 * we can center it on screen. glutBitmapWidth handles each character.
 */
static int text_width(const char *text, void *font)
{
    int total = 0;
    const char *c;
    for (c = text; *c != '\0'; c++)
        total += glutBitmapWidth(font, *c);
    return total;
}

/**
 * Full-screen overlay shown after the player has either won or died.
 * Tints the world with a soft color and shows the headline + an
 * "ESC to quit" prompt.
 */
static void draw_endscreen(t_game_state state)
{
    const char *headline;
    float r;
    float g;
    float b;
    int w_head;
    int w_prompt;
    const char *prompt = "Press ESC to quit";

    if (state == GAME_WON)
    {
        headline = "YOU ESCAPED";
        r = 0.0f; g = 0.6f; b = 0.1f;
    }
    else
    {
        headline = "YOU DIED";
        r = 0.6f; g = 0.0f; b = 0.0f;
    }

    /* Blended tint over the rendered scene. */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, 0.55f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f((float)SCREEN_WIDTH, 0.0f);
    glVertex2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
    glVertex2f(0.0f, (float)SCREEN_HEIGHT);
    glEnd();
    glDisable(GL_BLEND);

    w_head = text_width(headline, GLUT_BITMAP_TIMES_ROMAN_24);
    w_prompt = text_width(prompt, GLUT_BITMAP_HELVETICA_18);

    glColor3f(1.0f, 1.0f, 1.0f);
    draw_text_big((SCREEN_WIDTH - w_head) * 0.5f, SCREEN_HEIGHT * 0.55f, headline);
    draw_text((SCREEN_WIDTH - w_prompt) * 0.5f, SCREEN_HEIGHT * 0.45f, prompt);
}

/**
 * Shows the "press F to pick up key" prompt when the player is on the
 * key tile and the key has not been collected yet.
 */
static void draw_pickup_popup(t_game *game)
{
    int player_tile_x;
    int player_tile_y;
    const char *prompt;

    if (game->has_key)
        return;

    player_tile_x = (int)game->player->player_x;
    player_tile_y = (int)game->player->player_y;
    if (player_tile_x != game->map.key_x || player_tile_y != game->map.key_y)
        return;

    prompt = "Press F to pick up the key";

    /* Dim background bar so the white text is readable against the world. */
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.10f);
    glVertex2f(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.10f);
    glVertex2f(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.16f);
    glVertex2f(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.16f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    draw_text(SCREEN_WIDTH * 0.30f, SCREEN_HEIGHT * 0.12f, prompt);
}

/**
 * Lazily load enemy sprites on the first frame we render. Texture
 * creation requires a live GL context, which only exists after GLUT
 * has created the window, so we cannot do this inside init_game.
 */
static void ensure_enemy_sprites_loaded(t_game *game)
{
    if (game->enemy_sprites.loaded)
        return;
    if (!enemy_sprites_load(&game->enemy_sprites))
        fprintf(stderr, "Warning: enemy sprites failed to load; enemies will not be drawn.\n");
}

/**
 * Project an enemy from world space into camera space relative to the
 * player. cam_forward is depth (always positive when the enemy is in
 * front of the player), cam_right is the lateral offset used to compute
 * the screen-x of the sprite's center.
 */
static void enemy_to_camera_space(const t_player *player, const t_enemy *enemy,
                                  float *cam_forward, float *cam_right)
{
    float dx = enemy->enemy_x - player->player_x;
    float dy = enemy->enemy_y - player->player_y;
    float c = cosf(player->player_angle);
    float s = sinf(player->player_angle);
    *cam_forward = dx * c + dy * s;
    *cam_right   = -dx * s + dy * c;
}

/**
 * Render a single enemy sprite column-by-column. For each screen
 * column the sprite spans we sample the correct texture u and skip the
 * column if a wall is closer than the enemy on that column - this is
 * the classic raycaster occlusion trick.
 */
static void render_one_enemy(t_game *game, const t_enemy *enemy,
                             const float *wall_distances, int screen_w)
{
    const t_sprite *frame;
    float cam_forward;
    float cam_right;
    float fov;
    float sprite_screen_x;
    float sprite_size;
    float left;
    float right;
    float top;
    float bottom;
    int col_start;
    int col_end;
    int col;

    frame = enemy_sprites_select_frame(&game->enemy_sprites, enemy);
    if (frame == NULL || frame->texture_id == 0)
        return;

    enemy_to_camera_space(game->player, enemy, &cam_forward, &cam_right);
    if (cam_forward <= 0.05f) /* behind the player / too close to project safely */
        return;

    fov = raycast_get_fov_radians();

    /* Use the same projection convention raycast.c uses:
         angle_offset = (x / screen_w) - 0.5
         ray_angle    = player_angle + angle_offset * fov
       so an enemy at lateral angle (cam_right / cam_forward) lands at:
         x = ((atan(cam_right / cam_forward) / fov) + 0.5) * screen_w
    */
    sprite_screen_x = (atan2f(cam_right, cam_forward) / fov + 0.5f) * (float)screen_w;

    /* Match the wall scaling (see draw_wall_slice in raycast.c).
       Sprite assumed square; scale uniformly off cam_forward. */
    sprite_size = (SCREEN_HEIGHT / cam_forward) * 0.5f;
    if (sprite_size < 1.0f)
        return;

    left   = sprite_screen_x - sprite_size * 0.5f;
    right  = sprite_screen_x + sprite_size * 0.5f;
    top    = (SCREEN_HEIGHT + sprite_size) * 0.5f;
    bottom = (SCREEN_HEIGHT - sprite_size) * 0.5f;

    col_start = (int)floorf(left);
    col_end   = (int)ceilf(right);
    if (col_start < 0)
        col_start = 0;
    if (col_end > screen_w)
        col_end = screen_w;
    if (col_start >= col_end)
        return;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, frame->texture_id);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    col = col_start;
    while (col < col_end)
    {
        /* Skip columns occluded by a wall closer than this enemy. */
        if (wall_distances[col] > 0.0f && wall_distances[col] < cam_forward)
        {
            col++;
            continue;
        }

        /* Map this screen column back to a u coordinate in the sprite. */
        float u = ((float)col - left) / sprite_size;
        if (u < 0.0f) u = 0.0f;
        if (u > 1.0f) u = 1.0f;
        float u_next = ((float)(col + 1) - left) / sprite_size;
        if (u_next < 0.0f) u_next = 0.0f;
        if (u_next > 1.0f) u_next = 1.0f;

        float x0 = (float)col;
        float x1 = (float)(col + 1);

        /* Texture origin: stb_image gives us rows top-down, OpenGL's
           default UV has v=0 at the bottom. So flip v: top of sprite
           is v=0 here, bottom is v=1. */
        glTexCoord2f(u,      0.0f); glVertex2f(x0, top);
        glTexCoord2f(u_next, 0.0f); glVertex2f(x1, top);
        glTexCoord2f(u_next, 1.0f); glVertex2f(x1, bottom);
        glTexCoord2f(u,      1.0f); glVertex2f(x0, bottom);

        col++;
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

/**
 * Draw a yellow beam from a "gun barrel" point at the bottom-center of
 * the screen toward the projected world position of the most recent
 * shot endpoint. Alpha fades with the remaining shot_beam_timer.
 */
static void draw_shot_beam(t_game *game)
{
    float remaining;
    float cam_forward;
    float cam_right;
    float fov;
    float screen_x;
    float screen_y;
    float dx;
    float dy;
    float c;
    float s;
    float alpha;

    remaining = game->player->shot_beam_timer;
    if (remaining <= 0.0f)
        return;

    dx = game->player->shot_endpoint_x - game->player->player_x;
    dy = game->player->shot_endpoint_y - game->player->player_y;
    c = cosf(game->player->player_angle);
    s = sinf(game->player->player_angle);
    cam_forward = dx * c + dy * s;
    cam_right   = -dx * s + dy * c;
    if (cam_forward < 0.05f)
        return; /* shot pointed backwards somehow; nothing to draw */

    fov = raycast_get_fov_radians();
    screen_x = (atan2f(cam_right, cam_forward) / fov + 0.5f) * (float)SCREEN_WIDTH;
    screen_y = (float)SCREEN_HEIGHT * 0.5f;

    alpha = remaining / PLAYER_SHOT_BEAM_SECONDS;
    if (alpha > 1.0f) alpha = 1.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(3.0f);
    glColor4f(1.0f, 0.9f, 0.2f, alpha);
    glBegin(GL_LINES);
    glVertex2f((float)SCREEN_WIDTH * 0.5f, 0.0f); /* gun barrel point */
    glVertex2f(screen_x, screen_y);
    glEnd();
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}

/**
 * Health bar in the bottom-left. Width scales with current health,
 * color shifts red -> orange -> green as HP increases for at-a-glance
 * status.
 */
static void draw_health_bar(t_game *game)
{
    const float bar_x = 16.0f;
    const float bar_y = 16.0f;
    const float bar_w = 220.0f;
    const float bar_h = 22.0f;
    float frac;
    float fill_w;
    float r;
    float g;
    float b;
    char label[32];

    frac = game->player->health / (float)PLAYER_MAX_HEALTH;
    if (frac < 0.0f) frac = 0.0f;
    if (frac > 1.0f) frac = 1.0f;
    fill_w = bar_w * frac;

    /* Red below 33%, orange to 66%, green above. */
    if (frac < 0.33f)      { r = 0.85f; g = 0.10f; b = 0.10f; }
    else if (frac < 0.66f) { r = 0.95f; g = 0.55f; b = 0.05f; }
    else                   { r = 0.20f; g = 0.80f; b = 0.20f; }

    /* Background. */
    glColor3f(0.12f, 0.12f, 0.12f);
    glBegin(GL_QUADS);
    glVertex2f(bar_x,         bar_y);
    glVertex2f(bar_x + bar_w, bar_y);
    glVertex2f(bar_x + bar_w, bar_y + bar_h);
    glVertex2f(bar_x,         bar_y + bar_h);
    glEnd();

    /* Filled portion. */
    if (fill_w > 0.0f)
    {
        glColor3f(r, g, b);
        glBegin(GL_QUADS);
        glVertex2f(bar_x,          bar_y);
        glVertex2f(bar_x + fill_w, bar_y);
        glVertex2f(bar_x + fill_w, bar_y + bar_h);
        glVertex2f(bar_x,          bar_y + bar_h);
        glEnd();
    }

    /* Border. */
    glColor3f(0.75f, 0.75f, 0.75f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(bar_x,         bar_y);
    glVertex2f(bar_x + bar_w, bar_y);
    glVertex2f(bar_x + bar_w, bar_y + bar_h);
    glVertex2f(bar_x,         bar_y + bar_h);
    glEnd();

    snprintf(label, sizeof(label), "HP %d / %d",
             (int)(game->player->health + 0.5f), PLAYER_MAX_HEALTH);
    glColor3f(1.0f, 1.0f, 1.0f);
    draw_text(bar_x + bar_w + 10.0f, bar_y + 4.0f, label);
}

void render_enemies(t_game *game)
{
    const float *wall_distances;
    int screen_w;
    int i;

    ensure_enemy_sprites_loaded(game);
    if (!game->enemy_sprites.loaded)
        return;

    wall_distances = raycast_get_wall_distances();
    screen_w = raycast_get_screen_width();

    i = 0;
    while (i < game->enemy_count)
    {
        if (game->enemies[i].alive)
            render_one_enemy(game, &game->enemies[i], wall_distances, screen_w);
        i++;
    }
}

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
    render_enemies(game);
    draw_shot_beam(game);
    draw_pickup_popup(game);
    draw_health_bar(game);

    if (game->state != GAME_PLAYING)
        draw_endscreen(game->state);

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
