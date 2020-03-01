#pragma once

#include "common.h"
#include "item.h"
#include "tiletypes.h"

#define LEVEL_NAME_LIMIT 32
#define LEVEL_SIZE 20

#define NUM_LEVELS 2
#define MAX_WIDTH 512
#define MAX_HEIGHT 512

typedef struct {
        int num_tile_sets;
        int *tile_map;                       // Stores tile types
        Texture texture_map[NUM_TILE_TYPES]; // Stores tile textures indexed
                                             // by tile type
        Color color;                         // Background color
        item_stack **item_map;
        char name[LEVEL_NAME_LIMIT];
        Vector2 starting_position;
} level;

level *level_load();
void level_destroy(level *l);
void level_render_background(level *l);
void level_render_tiles(level *l);
int tile_has_collision(int tiletype);
int level_should_reset(level *l, Vector2 character_position);
int level_validate_position(level *l, Vector2 position);
int level_tile_at(level *l, Vector2 position);

void level_get_path(char *buf, int id);
level *level_load_file(const char *filename);
