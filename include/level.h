#ifndef level_h
#define level_h

#include "corange.h"

#define LEVEL_NAME_LIMIT 32

typedef struct {
        int num_tiles;
        GLuint positions_buffer;
        GLuint texcoords_buffer;
        asset_hndl mat;
} tile_set;

typedef struct {
        int num_tile_sets;
        int *tile_map;
        tile_set *tile_sets;
        char name[LEVEL_NAME_LIMIT];
} level;

#define TILETYPE_NONE 0
#define TILETYPE_AIR 1

#define TILETYPE_DIRT 2
#define TILETYPE_DIRT_ROCK 3
#define TILETYPE_DIRT_OVERHANG 4

#define TILETYPE_SURFACE 5
#define TILETYPE_GRASS 6
#define TILETYPE_GRASS_ROCK1 7
#define TILETYPE_GRASS_ROCK2 8

#define TILETYPE_GRASS_TREE 9
#define TILETYPE_TREE 10
#define TILETYPE_TREE_TOP 11
#define TILETYPE_TREE_TOP_LEFT 12
#define TILETYPE_TREE_TOP_RIGHT 13
#define TILETYPE_TREE_TOPEST 14
#define TILETYPE_TREE_BOT_LEFT 15
#define TILETYPE_TREE_BOT_RIGHT 16
#define TILETYPE_TREE_JUNC_LEFT 17
#define TILETYPE_TREE_JUNC_RIGHT 18
#define TILETYPE_TREE_TURN_LEFT 19
#define TILETYPE_TREE_TURN_RIGHT 20
#define TILETYPE_TREE_SIDE 21

#define TILETYPE_HOUSE_TOP_LEFT 22
#define TILETYPE_HOUSE_TOP_RIGHT 23
#define TILETYPE_HOUSE_BOT_LEFT 24
#define TILETYPE_HOUSE_BOT_RIGHT 25

#define TILETYPE_BRICK 26
#define TILETYPE_BRICK_WITH_FLOOR 27
#define TILETYPE_DOOR 28
#define TILETYPE_DOOR_OPEN 29
#define TILETYPE_LADDER 30
#define TILETYPE_WALKWAY_TOP 31
#define TILETYPE_WALKWAY_BOT 32
#define TILETYPE_COBWEB 33
#define TILETYPE_IVY 34

#define NUM_TILE_TYPES 35 // Always one more than the last tiletype

#define TILE_SIZE 32 // 8x8 pixel tiles

level *level_load_file(const char *filename);
void level_delete(level *l);

void level_render_background(level *l);
void level_render_tiles(level *l, vec2 camera_position);

int level_tile_at(level *l, vec2 position);
vec2 level_tile_position(level *l, int x, int y);

bool tile_has_collision(int tiletype);

#endif
