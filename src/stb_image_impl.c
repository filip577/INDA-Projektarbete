/*
 * stb_image_impl.c
 *
 * stb_image.h is a header-only library; defining STB_IMAGE_IMPLEMENTATION
 * in exactly one translation unit pulls in the actual function bodies.
 * This file is that translation unit - keep it tiny.
 *
 * The pragma block silences the -Wall/-Wextra/-Werror warnings stb_image
 * generates (unused functions, sign comparisons, etc.) without weakening
 * warnings for the rest of the project.
 */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG          /* only PNG is needed; trims binary size */
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#include "../include/stb_image.h"

#pragma GCC diagnostic pop
