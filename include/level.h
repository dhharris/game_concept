#ifndef level_h
#define level_h

#include "corange.h"
#include "item.h"

#define LEVEL_NAME_LIMIT 32
#define NUM_LEVELS 32

typedef struct {
        int num_tiles;
        GLuint positions_buffer;
        GLuint texcoords_buffer;
        asset_hndl mat;
} tile_set;

typedef struct {
        int num_tile_sets;
        int *tile_map;
        vec3 color;
        tile_set *tile_sets;
        item_stack **item_map;
        char name[LEVEL_NAME_LIMIT];
        vec2 character_position;
} level;

#define MAX_WIDTH 512
#define MAX_HEIGHT 512
#define LEVEL_SIZE 20


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

#define TILE_SIZE 16 // 16x16 pixel tiles

void level_get_path(char *buf, int num);
level *level_load_file(const char *filename);
void level_destroy(level *l);

void level_render_background(level *l);
void level_render_tiles(level *l, vec2 camera_position);

int level_tile_at(level *l, vec2 position);
vec2 level_tile_position(level *l, int x, int y);

int tile_has_collision(int tiletype);

#endif
