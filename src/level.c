#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raymath.h"
#include "utils.h"

#include "level.h"

Texture2D tile_get_texture(int tiletype)
{
        switch (tiletype) {
                case TILETYPE_AIR:
                        return LoadTexture("./sprites/sprite-0-13.png");
                case TILETYPE_DIRT:
                case TILETYPE_DIRT_OVERHANG:
                case TILETYPE_SURFACE:
                        return LoadTexture("./sprites/sprite-8-3.png");
                case TILETYPE_DIRT_ROCK:
                        return LoadTexture("./sprites/sprite-8-4.png");
                case TILETYPE_GRASS:
                        return LoadTexture("./sprites/sprite-11-4.png");
                case TILETYPE_GRASS_ROCK1:
                case TILETYPE_GRASS_ROCK2:
                        return LoadTexture("./sprites/sprite-11-5.png");
                case TILETYPE_TREE:
                        return LoadTexture("./sprites/sprite-12-3.png");
                case TILETYPE_BRICK:
                        return LoadTexture("./sprites/sprite-8-0.png");
                case TILETYPE_DOOR:
                        return LoadTexture("./sprites/sprite-8-12.png");
                case TILETYPE_DOOR_OPEN:
                        return LoadTexture("./sprites/sprite-8-13.png");
                case TILETYPE_STAIRS_DOWN:
                        return LoadTexture("./sprites/sprite-8-10.png");
                case TILETYPE_STAIRS_UP:
                        return LoadTexture("./sprites/sprite-8-11.png");
                case TILETYPE_COBWEB:
                        return LoadTexture("./sprites/sprite-11-1.png");
                default:
                        return LoadTexture("./sprites/sprite-0-13.png");
        }
}

Color tile_get_color(int tiletype)
{
        float transparency_factor;
        // All tiles are just a little darker than the background color,
        // so we use transparency tricks
        switch (tiletype) {
                case TILETYPE_DOOR:
                case TILETYPE_DOOR_OPEN:
                case TILETYPE_STAIRS_UP:
                case TILETYPE_STAIRS_DOWN:
                        transparency_factor = 0.25;
                case TILETYPE_BRICK:
                        transparency_factor = 0.75;
                default:
                        transparency_factor = 0.5;
        }
        return (Color){0, 0, 0, transparency_factor * 255};
}

int tile_has_collision(int tiletype)
{
        /* Blacklist of tiles that have collision enabled */
        switch (tiletype) {
                case TILETYPE_DIRT:
                        return 1;
                case TILETYPE_DIRT_ROCK:
                        return 1;
                case TILETYPE_DIRT_OVERHANG:
                        return 1;
                case TILETYPE_SURFACE:
                        return 1;
                case TILETYPE_GRASS_ROCK1:
                        return 1;
                case TILETYPE_BRICK:
                        return 1;
                case TILETYPE_DOOR:
                        return 1;
        }

        return 0;
}

/**
 * Levels are stored in an ascii file. This method defines what those
 * characters map to.
 */
static int char_to_tile(char c)
{
        switch (c) {
                case '\r':
                case '\n':
                case ' ':
                        return TILETYPE_NONE;
                case '`':
                        return TILETYPE_AIR;
                case '#':
                        return TILETYPE_BRICK;
                case 'D':
                        return TILETYPE_DIRT;
                case 'R':
                        return TILETYPE_DIRT_ROCK;
                case '+':
                        return TILETYPE_DOOR;
                case '-':
                        return TILETYPE_DOOR_OPEN;
                case '>':
                        return TILETYPE_STAIRS_DOWN;
                case '<':
                        return TILETYPE_STAIRS_UP;
                case '"':
                        return TILETYPE_GRASS;
                case '^':
                        return TILETYPE_TREE;
                default:
                        return TILETYPE_NONE;
        }
}

static int tile_counts[NUM_TILE_TYPES];

/* Load the num level into buf */
void level_get_path(char *buf, int num)
{
        snprintf(buf, LEVEL_NAME_LIMIT, "./levels/%d.level", num);
}

level *level_load_file(const char *filename)
{
        FILE *file = fopen(filename, "r");

        if (!file) {
                TRACELOG(LOG_ERROR, "Failed to open file: %s", filename);
                return NULL;
        }

        // Reset tile counts
        memset(tile_counts, 0, sizeof(tile_counts));

        level *l = malloc(sizeof(level));
        l->tile_map = malloc(sizeof(int) * MAX_WIDTH * MAX_HEIGHT);
        l->item_map = item_map_init();
        l->character_position = Vector2Zero();

        int y = 0;
        int x = 0;
        char *line;
        size_t linecap = 0;
        ssize_t linelen;
        while ((linelen = getline(&line, &linecap, file)) > 0) {
                for (x = 0; x < linelen; x++) {
                        char c = line[x];
                        int type = char_to_tile(c);

                        /* Character starts at the up stairwell */
                        if (type == TILETYPE_STAIRS_UP)
                                l->character_position =
                                    Vector2Scale((Vector2){x, y}, TILE_SIZE);

                        l->tile_map[x + y * MAX_WIDTH] = type;
                        tile_counts[type]++;
                }
                ++y;
        }

        fclose(file);

        // Populate tile textures used in this level
        for (int i = 1; i < NUM_TILE_TYPES; i++) {
                if (tile_counts[i])
                        l->texture_map[i] = tile_get_texture(i);
        }

        // Load level name, placing null byte when encountering the .
        const char *start = filename + strlen(filename);
        while (*start-- != '/')
                ; // NULL
        strncpy(l->name, start + 2, LEVEL_NAME_LIMIT);
        l->name[LEVEL_NAME_LIMIT - 1] = '\0';
        for (int i = 0; i < LEVEL_NAME_LIMIT; ++i) {
                if (l->name[i] == '.') {
                        l->name[i] = '\0';
                        break;
                }
        }

        // Greenish color
        l->color = (Color){71, 122, 64, 255};

        return l;
}

void level_destroy(level *l)
{
        free(l->tile_map);
        item_map_destroy(l->item_map);
        for (int i = 0; i < NUM_TILE_TYPES; i++) {
                if (tile_counts[i])
                        UnloadTexture(l->texture_map[i]);
        }
        free(l);
}

/* Render rectangle of level based on its color and size */
void level_render_background(level *l)
{
        DrawRectangle(0, 0, LEVEL_SIZE * TILE_SIZE, LEVEL_SIZE * TILE_SIZE,
                      l->color);
}

void level_render_tiles(level *l)
{
        for (int x = 0; x < LEVEL_SIZE; ++x) {
                for (int y = 0; y < LEVEL_SIZE; ++y) {
                        int type = l->tile_map[x + y * MAX_WIDTH];
                        DrawTextureV(l->texture_map[type],
                                     Vector2Scale((Vector2){x, y}, TILE_SIZE),
                                     tile_get_color(type));
                }
        }
}

int level_tile_at(level *l, Vector2 position)
{
        int x = floor(position.x / TILE_SIZE);
        int y = floor(position.y / TILE_SIZE);

        assert(x >= 0);
        assert(y >= 0);
        assert(x < MAX_WIDTH);
        assert(y < MAX_HEIGHT);

        return l->tile_map[x + y * MAX_WIDTH];
}
