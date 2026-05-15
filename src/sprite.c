#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include "../include/stb_image.h"
#include "../include/sprite.h"

/*
 * Configure GL texture parameters for pixel-art sprites:
 *  - GL_NEAREST so the sprites stay crisp instead of blurring
 *  - GL_CLAMP_TO_EDGE so we don't sample the opposite edge at u=1.0
 */
static void apply_sprite_tex_params(void)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool sprite_load(t_sprite *out, const char *path)
{
    int width;
    int height;
    int channels;
    unsigned char *pixels;
    GLuint texture_id;

    out->texture_id = 0;
    out->width = 0;
    out->height = 0;

    /* Force 4 channels (RGBA) so we always have an alpha channel even
       if the PNG was saved without one. */
    pixels = stbi_load(path, &width, &height, &channels, 4);
    if (!pixels)
    {
        fprintf(stderr, "Error: failed to load sprite '%s': %s\n",
                path, stbi_failure_reason());
        return false;
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    apply_sprite_tex_params();
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(pixels);

    out->texture_id = texture_id;
    out->width = width;
    out->height = height;
    return true;
}

void sprite_free(t_sprite *sprite)
{
    if (sprite->texture_id != 0)
    {
        glDeleteTextures(1, &sprite->texture_id);
        sprite->texture_id = 0;
    }
    sprite->width = 0;
    sprite->height = 0;
}

bool enemy_sprites_load(t_enemy_sprites *sprites)
{
    memset(sprites, 0, sizeof(*sprites));

    if (!sprite_load(&sprites->idle,   "assets/sprites/enemies/idle.png")   ||
        !sprite_load(&sprites->damage, "assets/sprites/enemies/damage.png") ||
        !sprite_load(&sprites->walk1,  "assets/sprites/enemies/walk1.png")  ||
        !sprite_load(&sprites->walk2,  "assets/sprites/enemies/walk2.png"))
    {
        enemy_sprites_free(sprites);
        return false;
    }

    sprites->loaded = true;
    return true;
}

void enemy_sprites_free(t_enemy_sprites *sprites)
{
    sprite_free(&sprites->idle);
    sprite_free(&sprites->damage);
    sprite_free(&sprites->walk1);
    sprite_free(&sprites->walk2);
    sprites->loaded = false;
}

/*
 * Frame selection rules:
 *   - damage_flash_timer > 0    -> damage.png
 *   - walking (walk_phase > 0)  -> walk1 / walk2 depending on phase
 *   - otherwise                 -> idle.png
 * Dead enemies are filtered out by the caller, so they never reach here.
 */
const t_sprite *enemy_sprites_select_frame(const t_enemy_sprites *sprites,
                                           const t_enemy *enemy)
{
    if (enemy->damage_flash_timer > 0.0f)
        return &sprites->damage;

    if (enemy->walk_phase > 0.0f)
    {
        /* walk_phase is kept in [0,1); first half shows walk1, second walk2. */
        return (enemy->walk_phase < 0.5f) ? &sprites->walk1 : &sprites->walk2;
    }

    return &sprites->idle;
}
