#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raymath.h"

#include "level.h"

/* These vast case statements are basically a nasty way of assigning properties
 * to the tile types */

Texture2D *tile_get_texture(int tiletype)
{
        //        texture *t;
        //        switch (tiletype) {
        //                case TILETYPE_AIR:
        //                        t = asset_get(P("./sprites/sprite-0-13.dds"));
        //                        break;
        //                case TILETYPE_DIRT:
        //                case TILETYPE_DIRT_OVERHANG:
        //                case TILETYPE_SURFACE:
        //                        t = asset_get(P("./sprites/sprite-8-3.dds"));
        //                        break;
        //                case TILETYPE_DIRT_ROCK:
        //                        t = asset_get(P("./sprites/sprite-8-4.dds"));
        //                        break;
        //                case TILETYPE_GRASS:
        //                        t = asset_get(P("./sprites/sprite-11-4.dds"));
        //                        break;
        //                case TILETYPE_GRASS_ROCK1:
        //                case TILETYPE_GRASS_ROCK2:
        //                        t = asset_get(P("./sprites/sprite-11-5.dds"));
        //                        break;
        //                case TILETYPE_TREE:
        //                        t = asset_get(P("./sprites/sprite-12-3.dds"));
        //                        break;
        //                case TILETYPE_BRICK:
        //                        t = asset_get(P("./sprites/sprite-8-0.dds"));
        //                        break;
        //                case TILETYPE_DOOR:
        //                        t = asset_get(P("./sprites/sprite-8-12.dds"));
        //                        break;
        //                case TILETYPE_DOOR_OPEN:
        //                        t = asset_get(P("./sprites/sprite-8-13.dds"));
        //                        break;
        //                case TILETYPE_STAIRS_DOWN:
        //                        t = asset_get(P("./sprites/sprite-8-10.dds"));
        //                        break;
        //                case TILETYPE_STAIRS_UP:
        //                        t = asset_get(P("./sprites/sprite-8-11.dds"));
        //                        break;
        //                case TILETYPE_COBWEB:
        //                        t = asset_get(P("./sprites/sprite-11-1.dds"));
        //                        break;
        //                default:
        //                        t = asset_get(P("./sprites/sprite-0-13.dds"));
        //                        break;
        //        }
        //        return t;
        return 0;
}

float tile_get_transparency(int tiletype)
{
        switch (tiletype) {
                case TILETYPE_DOOR:
                case TILETYPE_DOOR_OPEN:
                case TILETYPE_STAIRS_UP:
                case TILETYPE_STAIRS_DOWN:
                        return 0.25;
                case TILETYPE_BRICK:
                        return 0.75;
                default:
                        return 0.5;
        }
}

int tile_has_collision(int tiletype)
{

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

/* Levels are basically stored in an ascii file, with these being the tile type
 * characters. */

static int char_to_tile(char c)
{

        switch (c) {
                case '\r':
                        return TILETYPE_NONE;
                case '\n':
                        return TILETYPE_NONE;
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
        }

        return TILETYPE_NONE;
}

static int tile_counts[NUM_TILE_TYPES];

/* Load the num level into buf */
void level_get_path(char *buf, int num)
{
        snprintf(buf, LEVEL_NAME_LIMIT, "./levels/%d.level", num);
}

level *level_load_file(const char *filename)
{
        //
        //        int i;
        //
        //        for (i = 0; i < NUM_TILE_TYPES; i++) {
        //                tile_counts[i] = 0;
        //        }
        //
        //        level *l = malloc(sizeof(level));
        //        l->tile_sets = malloc(sizeof(tile_set) * NUM_TILE_TYPES);
        //        l->tile_map = calloc(sizeof(int), MAX_WIDTH * MAX_HEIGHT);
        //        l->item_map = item_map_init();
        //        l->character_position = Vector2Zero();
        //
        //        /* Load level name, placing null byte when encountering the .
        //        */ const char *start = filename + strlen(filename); while
        //        (*start-- != '/')
        //                ; // Find the part past the last /
        //        strncpy(l->name, start + 2, LEVEL_NAME_LIMIT);
        //        l->name[LEVEL_NAME_LIMIT - 1] = '\0';
        //        for (i = 0; i < LEVEL_NAME_LIMIT; ++i)
        //                if (l->name[i] == '.') {
        //                        l->name[i] = '\0';
        //                        break;
        //                }
        //
        //        SDL_RWops *file = SDL_RWFromFile(filename, "r");
        //        char line[MAX_WIDTH];
        //
        //        int y = 0;
        //        int x = 0;
        //        while (SDL_RWreadline(file, line, 1024)) {
        //
        //                for (x = 0; x < strlen(line); x++) {
        //                        char c = line[x];
        //                        int type = char_to_tile(c);
        //
        //                        /* Character starts at the up stairwell */
        //                        if (type == TILETYPE_STAIRS_UP)
        //                                l->character_position =
        //                                    Vector2_mul(Vector2_new(x, y),
        //                                    TILE_SIZE);
        //
        //                        l->tile_map[x + y * MAX_WIDTH] = type;
        //                        tile_counts[type]++;
        //                }
        //
        //                y++;
        //        }
        //
        //        SDL_RWclose(file);
        //
        //        /* Set character position to 1,1 if no stairs were found in
        //        the
        //         * level */
        //        if (Vector2_equ(l->character_position, Vector2Zero()))
        //                l->character_position = Vector2_mul(Vector2_new(1, 1),
        //                TILE_SIZE);
        //
        //        /* Start from 1, type 0 is none! */
        //        for (int i = 1; i < NUM_TILE_TYPES; i++) {
        //
        //                int num_tiles = tile_counts[i];
        //
        //                float *position_data =
        //                    malloc(sizeof(float) * 3 * 4 * num_tiles);
        //                float *uv_data = malloc(sizeof(float) * 2 * 4 *
        //                num_tiles);
        //
        //                int pos_i = 0;
        //                int uv_i = 0;
        //
        //                for (x = 0; x < MAX_WIDTH; x++)
        //                        for (y = 0; y < MAX_HEIGHT; y++) {
        //                                int type = l->tile_map[x + y *
        //                                MAX_WIDTH]; if (type == i) {
        //                                        position_data[pos_i] = x *
        //                                        TILE_SIZE; pos_i++;
        //                                        position_data[pos_i] = y *
        //                                        TILE_SIZE; pos_i++;
        //                                        position_data[pos_i] = 0;
        //                                        pos_i++;
        //
        //                                        position_data[pos_i] =
        //                                            (x + 1) * TILE_SIZE;
        //                                        pos_i++;
        //                                        position_data[pos_i] = y *
        //                                        TILE_SIZE; pos_i++;
        //                                        position_data[pos_i] = 0;
        //                                        pos_i++;
        //
        //                                        position_data[pos_i] =
        //                                            (x + 1) * TILE_SIZE;
        //                                        pos_i++;
        //                                        position_data[pos_i] =
        //                                            (y + 1) * TILE_SIZE;
        //                                        pos_i++;
        //                                        position_data[pos_i] = 0;
        //                                        pos_i++;
        //
        //                                        position_data[pos_i] = x *
        //                                        TILE_SIZE; pos_i++;
        //                                        position_data[pos_i] =
        //                                            (y + 1) * TILE_SIZE;
        //                                        pos_i++;
        //                                        position_data[pos_i] = 0;
        //                                        pos_i++;
        //
        //                                        uv_data[uv_i] = 0;
        //                                        uv_i++;
        //                                        uv_data[uv_i] = 0;
        //                                        uv_i++;
        //
        //                                        uv_data[uv_i] = 1;
        //                                        uv_i++;
        //                                        uv_data[uv_i] = 0;
        //                                        uv_i++;
        //
        //                                        uv_data[uv_i] = 1;
        //                                        uv_i++;
        //                                        uv_data[uv_i] = 1;
        //                                        uv_i++;
        //
        //                                        uv_data[uv_i] = 0;
        //                                        uv_i++;
        //                                        uv_data[uv_i] = 1;
        //                                        uv_i++;
        //                                }
        //                        }
        //
        //                l->tile_sets[i].num_tiles = num_tiles;
        //
        //                glGenBuffers(1, &l->tile_sets[i].positions_buffer);
        //                glGenBuffers(1, &l->tile_sets[i].texcoords_buffer);
        //
        //                glBindBuffer(GL_ARRAY_BUFFER,
        //                l->tile_sets[i].positions_buffer);
        //                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 *
        //                num_tiles,
        //                             position_data, GL_STATIC_DRAW);
        //
        //                glBindBuffer(GL_ARRAY_BUFFER,
        //                l->tile_sets[i].texcoords_buffer);
        //                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4 *
        //                num_tiles,
        //                             uv_data, GL_STATIC_DRAW);
        //
        //                 glBindBuffer(GL_ARRAY_BUFFER, 0);
        //
        //                 free(position_data);
        //                 free(uv_data);
        //         }
        //
        //         // Set level color
        //         l->color = vec3_new(0.28, 0.48, 0.25);
        //
        //         return l;
        return 0;
}

void level_destroy(level *l)
{

        //         /* Start from 1 as 0 is none tile set */
        //         for (int i = 1; i < NUM_TILE_TYPES; i++) {
        //                 glDeleteBuffers(1,
        //                 &l->tile_sets[i].positions_buffer);
        //                 glDeleteBuffers(1,
        //                 &l->tile_sets[i].texcoords_buffer);
        //         }
        //
        free(l->tile_map);
        free(l->tile_sets);
        item_map_destroy(l->item_map);
        free(l);
}

/* Render background of level based on its color */
void level_render_background(level *l)
{
        //
        //         glMatrixMode(GL_PROJECTION);
        //         glPushMatrix();
        //         glLoadIdentity();
        //         glOrtho(0, graphics_viewport_width(), 0,
        //         graphics_viewport_height(), -1,
        //                 1);
        //
        //         glMatrixMode(GL_MODELVIEW);
        //         glPushMatrix();
        //         glLoadIdentity();
        //
        //         glColor3f(l->color.x, l->color.y, l->color.z);
        //         glBegin(GL_QUADS);
        //
        //         glVertex3f(0, graphics_viewport_height(), 0.0);
        //         glTexCoord2f(1, 0);
        //         glVertex3f(graphics_viewport_width(),
        //         graphics_viewport_height(), 0.0); glTexCoord2f(1, 1);
        //         glVertex3f(graphics_viewport_width(), 0, 0.0);
        //         glTexCoord2f(0, 1);
        //         glVertex3f(0, 0, 0.0);
        //         glTexCoord2f(0, 0);
        //
        //         glEnd();
        //
        //         glMatrixMode(GL_PROJECTION);
        //         glPopMatrix();
        //
        //         glMatrixMode(GL_MODELVIEW);
        //         glPopMatrix();
}

/* Renders each tileset in one go. Uses vertex buffers. */

void level_render_tiles(level *l, Vector2 camera_position)
{
        //
        //        glMatrixMode(GL_PROJECTION);
        //        glPushMatrix();
        //        glLoadIdentity();
        //        glOrtho(camera_position.x - graphics_viewport_width() / 2,
        //                camera_position.x + graphics_viewport_width() / 2,
        //                -camera_position.y + graphics_viewport_height() / 2,
        //                -camera_position.y - graphics_viewport_height() / 2,
        //                -1, 1);
        //
        //        glMatrixMode(GL_MODELVIEW);
        //        glPushMatrix();
        //        glLoadIdentity();
        //
        //        glEnable(GL_TEXTURE_2D);
        //
        //        glEnable(GL_BLEND);
        //        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //
        //        /* Start from 1, 0 is no tiles! */
        //
        //        for (int i = 1; i < NUM_TILE_TYPES; i++) {
        //                // Tile transparency
        //                glColor4f(1.0, 1.0, 1.0, tile_get_transparency(i));
        //
        //                texture *tile_tex = tile_get_texture(i);
        //                glBindTexture(GL_TEXTURE_2D,
        //                texture_handle(tile_tex));
        //                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        //                                GL_CLAMP_TO_EDGE);
        //                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
        //                                GL_CLAMP_TO_EDGE);
        //
        //                glEnableClientState(GL_VERTEX_ARRAY);
        //                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        //
        //                glBindBuffer(GL_ARRAY_BUFFER,
        //                l->tile_sets[i].positions_buffer); glVertexPointer(3,
        //                GL_FLOAT, 0, (void *)0);
        //
        //                glBindBuffer(GL_ARRAY_BUFFER,
        //                l->tile_sets[i].texcoords_buffer);
        //                glTexCoordPointer(2, GL_FLOAT, 0, (void *)0);
        //
        //                glDrawArrays(GL_QUADS, 0, l->tile_sets[i].num_tiles *
        //                4);
        //
        //                glBindBuffer(GL_ARRAY_BUFFER, 0);
        //                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        //                glDisableClientState(GL_VERTEX_ARRAY);
        //        }
        //
        //        glDisable(GL_BLEND);
        //        glDisable(GL_TEXTURE_2D);
        //
        //        glMatrixMode(GL_PROJECTION);
        //        glPopMatrix();
        //
        //        glMatrixMode(GL_MODELVIEW);
        //        glPopMatrix();
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

Vector2 level_tile_position(level *l, int x, int y)
{
        Vector2 pos = {x * TILE_SIZE, y * TILE_SIZE};
        return pos;
}
