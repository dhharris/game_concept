#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raymath.h"
#include "utils.h"

#include "helpers.h"
#include "level.h"

static int CURRENT_LEVEL_ID; // Level that the player is on
static int GOING_DOWN = 1;   // Whether the player is going down
static int tile_counts[NUM_TILE_TYPES];

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

/* Load the num level into buf */
void level_get_path(char *buf, int id)
{
        snprintf(buf, LEVEL_NAME_LIMIT, "./levels/%d.level", id);
}

level *level_load_file(const char *filename)
{
        FILE *file = fopen(filename, "r");

        if (!file) {
                TraceLog(LOG_ERROR, "[%s] Failed to open file: %s", __FILE__,
                         filename);
                return NULL;
        }

        TraceLog(LOG_INFO, "[%s] Loading level: %s", __FILE__, filename);

        // Reset tile counts
        memset(tile_counts, 0, sizeof(tile_counts));

        level *l = malloc(sizeof(level));
        // Make sure all tiles are initialized
        l->tile_map = calloc(LEVEL_SIZE * LEVEL_SIZE, sizeof(int));
        l->item_map = item_map_init();

        int y = 0;
        int x = 0;
        char *line = 0;
        size_t linecap = 0;
        ssize_t linelen;
        Vector2 stair_up_position = Vector2Zero();
        Vector2 stair_down_position = Vector2Zero();
        while ((linelen = getline(&line, &linecap, file)) > 0) {
                for (x = 0; x < linelen; x++) {
                        char c = line[x];
                        int type = char_to_tile(c);

                        if (type == TILETYPE_STAIRS_UP) {
                                stair_up_position =
                                    Vector2Scale((Vector2){x, y}, TILE_SIZE);
                        } else if (type == TILETYPE_STAIRS_DOWN) {
                                stair_down_position =
                                    Vector2Scale((Vector2){x, y}, TILE_SIZE);
                        }

                        l->tile_map[x + y * LEVEL_SIZE] = type;
                        tile_counts[type]++;
                }
                ++y;
        }
        // Determine which stairwell we arrive at upon loading level
        l->starting_position =
            GOING_DOWN ? stair_up_position : stair_down_position;

        free(line);
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

level *level_load()
{
        char path[LEVEL_NAME_LIMIT];
        level_get_path(path, CURRENT_LEVEL_ID);
        return level_load_file(path);
}

void level_destroy(level *l)
{
        if (l == 0) {
                return;
        }
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
                        int type = l->tile_map[x + y * LEVEL_SIZE];
                        if (type != TILETYPE_NONE) {
                                DrawTextureV(
                                    l->texture_map[type],
                                    Vector2Scale((Vector2){x, y}, TILE_SIZE),
                                    tile_get_color(type));
                        }
                }
        }
}

Vector2 level_get_position(level *l, Vector2 position)
{
        int x = fabs(floorf(position.x / TILE_SIZE));
        int y = fabs(floorf(position.y / TILE_SIZE));

        assert(x >= 0 && x < LEVEL_SIZE);
        assert(y >= 0 && y < LEVEL_SIZE);

        return (Vector2){x, y};
}

/* Returns a tile given level x, y coordinates */
int level_get_tile(level *l, int x, int y)
{
        return l->tile_map[y * LEVEL_SIZE + x];
}

/* Returns the tile at a given screen position */
int level_tile_at(level *l, Vector2 position)
{
        Vector2 pos = level_get_position(l, position);
        return level_get_tile(l, pos.x, pos.y);
}

int change_level(int delta)
{
        int new_level = CURRENT_LEVEL_ID + delta;
        if (new_level >= 0 && new_level < NUM_LEVELS) {
                GOING_DOWN = (delta > 0) ? 1 : 0;
                CURRENT_LEVEL_ID = new_level;
                return 1;
        }
        return 0;
}

int level_validate_position(level *l, Vector2 character_position)
{
        int tile = level_tile_at(l, character_position);
        return !tile_has_collision(tile);
}

/* Tell the main loop when to reset the level */
int level_should_reset(level *l, Vector2 character_position)
{
        switch (level_tile_at(l, character_position)) {
                case TILETYPE_STAIRS_UP:
                        return change_level(-1);
                case TILETYPE_STAIRS_DOWN:
                        return change_level(1);
                default:
                        return 0;
        }
}

int within_level_bounds(int x, int y)
{
        return ((x >= 0 && x < LEVEL_SIZE) && (y >= 0 && y < LEVEL_SIZE));
}

/* Computes shortest path using BFS */
path *level_shortest_path(level *l, Vector2 start, Vector2 end)
{
        // Early exit if end is invalid
        if (!level_validate_position(l, end)) {
                return path_new();
        }
        int visited[LEVEL_SIZE][LEVEL_SIZE] = {0};
        int distance_from_start[LEVEL_SIZE][LEVEL_SIZE] = {0};
        Vector2 level_start = level_get_position(l, start);
        Vector2 level_end = level_get_position(l, end);

        // Stores the parent of each node, as level coordinates
        Vector2 parent_map[LEVEL_SIZE][LEVEL_SIZE];
        for (int x = 0; x < LEVEL_SIZE; ++x) {
                for (int y = 0; y < LEVEL_SIZE; ++y) {
                        // Note that start node will have this as its parent
                        parent_map[x][y] = VECTOR2_NULL;
                }
        }
        path *queue = path_new();
        // Convert start to level pos and put in queue
        path_push(queue, level_start);
        visited[(int) level_start.x][(int) level_start.y] = 1;
        TraceLog(LOG_INFO, "[PATHFINDING] Computing shortest path between (%f, %f) and (%f, %f)", level_start.x, level_start.y, level_end.x, level_end.y);

        // Continue until queue is empty
        while (!path_empty(queue)) {
                Vector2 pos = path_pop(&queue);
                TraceLog(LOG_INFO, "[PATHFINDING] Processing (%f, %f)", pos.x, pos.y);
                if (Vector2Eq(pos, level_end)) {
                        break;
                }
                int x0 = pos.x;
                int y0 = pos.y;
                int tile = level_get_tile(l, x0, y0);
                if (tile_has_collision(tile)) {
                        // Cannot go to this tile, so it can't have a parent
                        parent_map[x0][y0] = VECTOR2_NULL;
                        continue;
                }

                // Add all neighbors to queue if not visited and in-bounds
                for (int x = x0 - 1; x <= x0 + 1; x++) {
                        for (int y = y0 - 1; y <= y0 + 1; y++) {
                                if (!within_level_bounds(x, y)) {
                                        continue;
                                }
                                if (!visited[x][y]) {
                                        visited[x][y] = 1;
                                        Vector2 parent = parent_map[x][y];
                                        distance_from_start[x][y] =
                                                distance_from_start[x0][y0] + 1;
                                        parent_map[x][y] = pos;
                                        Vector2 next = {x, y};
                                        path_push(queue, next);
                                }
                        }
                }
        }
        path_destroy(queue);

        // Reconstruct path using parent_map
        path *ret = path_new();
        int x = level_end.x;
        int y = level_end.y;
        while (!Vector2Eq(parent_map[x][y], VECTOR2_NULL)) {
                Vector2 curr = {x, y};
                // Need to scale to screen coordinates
                path_push_front(&ret, Vector2Scale(curr, TILE_SIZE));
                Vector2 parent = parent_map[x][y];
                x = parent.x;
                y = parent.y;
        }
        return ret;
}
