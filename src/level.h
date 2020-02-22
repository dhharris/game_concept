#pragma once

#include "common.h"
#include "item.h"

#define LEVEL_NAME_LIMIT 32
#define NUM_LEVELS 2

#define TILETYPE_NONE 0
#define TILETYPE_AIR 1

#define TILETYPE_DIRT 2
#define TILETYPE_DIRT_ROCK 3
#define TILETYPE_DIRT_OVERHANG 4

#define TILETYPE_SURFACE 5
#define TILETYPE_GRASS 6
#define TILETYPE_GRASS_ROCK1 7
#define TILETYPE_GRASS_ROCK2 8

#define TILETYPE_TREE 9
#define TILETYPE_TREE_PINE 10
#define TILETYPE_TREE_DEAD 11

#define TILETYPE_BRICK 12
#define TILETYPE_DOOR 13
#define TILETYPE_DOOR_OPEN 14
#define TILETYPE_STAIRS_UP 15
#define TILETYPE_STAIRS_DOWN 16
#define TILETYPE_COBWEB 17

#define NUM_TILE_TYPES 18 // Always one more than the last tiletype

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

#define MAX_WIDTH 512
#define MAX_HEIGHT 512
#define LEVEL_SIZE 20

level *level_load();
void level_destroy(level *l);
void level_render_background(level *l);
void level_render_tiles(level *l);
int tile_has_collision(int tiletype);
int level_should_reset(level *l, Vector2 character_position);
int level_tile_at(level *l, Vector2 position);

void level_get_path(char *buf, int id);
level *level_load_file(const char *filename);
