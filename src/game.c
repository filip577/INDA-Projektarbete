#include <stdio.h>
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include "../include/game.h"
#include "../include/input.h"
#include "../include/enemies.h"


/**
 * Method to connect map to player system, places player in middle of grid tile if tile length is 1
 */
void init_player_from_map(t_game *game)
{
    game->player->player_x = game->map.player_start_x + 0.5;
    game->player->player_y = game->map.player_start_y + 0.5;
    game->player->player_angle = 0.0; //starting angle
    game->player->movement_speed = 0.1; //starting rotation and movement speed
    game->player->rotation_speed = 0.1;
    game->player->health = (float)PLAYER_MAX_HEALTH;
    game->player->shoot_cooldown = 0.0f;
    game->player->shot_beam_timer = 0.0f;
    game->player->shot_endpoint_x = 0.0f;
    game->player->shot_endpoint_y = 0.0f;
}


/**
 * Method that initialises the game
 */
void init_game(t_game *game, const char *map_file)
{
    game->map = load_map_from_file(map_file); //loads map from file using map_load.c function

    game->player = malloc(sizeof(t_player));
    if (!game->player)
    {
        fprintf(stderr, "Error: failed to allocate player\n");
        free_map(&game->map);
        exit(EXIT_FAILURE);
    }
    game->input = &input;

    init_player_from_map(game); //initialises the player
    init_enemies(game);         //spawn one alive enemy per 'E' tile

    game->has_key = false;
    game->prev_time_ms = glutGet(GLUT_ELAPSED_TIME);
    game->enemy_sprites.loaded = false; //lazy-loaded on first render once GL context exists
    game->state = GAME_PLAYING;

    game->running = true; //game can run when true
}

/**
 * If the player is standing on the key tile and pressed F this frame,
 * mark the key as collected and replace the 'K' tile with a regular
 * floor so the pickup popup stops appearing.
 */
static void try_pickup_key(t_game *game, int player_tile_x, int player_tile_y)
{
    if (game->has_key)
        return;
    if (!game->input->pickup_pressed)
        return;
    if (player_tile_x != game->map.key_x || player_tile_y != game->map.key_y)
        return;

    game->has_key = true;
    game->map.grid[game->map.key_y][game->map.key_x] = '0';
    printf("Picked up the key! Find the exit (D) to escape.\n");
}

/**
 * Apply contact damage from any alive enemy within ENEMY_TOUCH_RANGE
 * to the player. Damage scales with dt so contact time matters rather
 * than the framerate.
 */
static void apply_enemy_contact_damage(t_game *game, float dt)
{
    int i;
    float dx;
    float dy;
    float range_sq = ENEMY_TOUCH_RANGE * ENEMY_TOUCH_RANGE;

    i = 0;
    while (i < game->enemy_count)
    {
        if (game->enemies[i].alive)
        {
            dx = game->enemies[i].enemy_x - game->player->player_x;
            dy = game->enemies[i].enemy_y - game->player->player_y;
            if (dx * dx + dy * dy < range_sq)
                game->player->health -= ENEMY_DPS * dt;
        }
        i++;
    }
}

/**
 * Process the shooting input. shoot_cooldown is decremented every frame;
 * when the space bar is held and the cooldown has elapsed we fire one
 * shot and reset the cooldown.
 */
static void handle_shooting(t_game *game, float dt)
{
    if (game->player->shoot_cooldown > 0.0f)
    {
        game->player->shoot_cooldown -= dt;
        if (game->player->shoot_cooldown < 0.0f)
            game->player->shoot_cooldown = 0.0f;
    }

    /* Tick the shot-beam fade every frame so the render side just needs
       to check shot_beam_timer > 0. */
    if (game->player->shot_beam_timer > 0.0f)
    {
        game->player->shot_beam_timer -= dt;
        if (game->player->shot_beam_timer < 0.0f)
            game->player->shot_beam_timer = 0.0f;
    }

    if (game->input->shooting && game->player->shoot_cooldown <= 0.0f)
    {
        player_shoot(game);
        game->player->shoot_cooldown = PLAYER_SHOOT_COOLDOWN_SECONDS;
    }
}

/**
 * Function that updates game state for one frame.
 *
 * Simulation only advances while state == GAME_PLAYING. On win/loss we
 * freeze the world and wait for the player to press ESC, which is the
 * single mechanism that ends game->running.
 */
void update_game(t_game *game)
{
    int player_tile_x;
    int player_tile_y;
    char current_tile;
    int now_ms;
    float dt;

    now_ms = glutGet(GLUT_ELAPSED_TIME);
    dt = (float)(now_ms - game->prev_time_ms) / 1000.0f;
    game->prev_time_ms = now_ms;
    if (dt < 0.0f)
        dt = 0.0f;

    /* ESC always quits, even from the win/lose screen. */
    if (game->input->quit_pressed)
    {
        game->input->quit_pressed = false;
        game->running = false;
        return;
    }

    if (game->state != GAME_PLAYING)
    {
        /* Consume any leftover one-shot inputs so they don't fire when a
           future game starts up. */
        game->input->pickup_pressed = false;
        return;
    }

    update_enemies(game, dt);
    apply_enemy_contact_damage(game, dt);
    handle_shooting(game, dt);

    if (game->player->health <= 0.0f)
    {
        game->player->health = 0.0f;
        game->state = GAME_LOST;
        printf("You died!\n");
        return;
    }

    player_tile_x = (int)game->player->player_x;
    player_tile_y = (int)game->player->player_y;

    try_pickup_key(game, player_tile_x, player_tile_y);
    game->input->pickup_pressed = false; //consume the one-shot pickup flag

    current_tile = get_tile(&game->map, player_tile_x, player_tile_y);
    if (current_tile == 'D' && game->has_key)
    {
        game->state = GAME_WON;
        printf("You escaped the labyrinth!\n");
    }
}


/**
 * Cleans up game, frees map from memory etc.
 */
void cleanup_game(t_game *game)
{
    enemy_sprites_free(&game->enemy_sprites);
    free_map(&game->map);
    if (game->player)
    {
        free(game->player);
        game->player = NULL;
    }
}
