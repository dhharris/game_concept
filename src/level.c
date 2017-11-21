#include <math.h>
#include <string.h>

#include "level.h"

const int MAX_WIDTH = 512;
const int MAX_HEIGHT = 512;

/* These vast case statements are basically a nasty way of assigning properties
 * to the tile types */

static texture *tile_get_texture(int tiletype)
{
        texture *t;
        switch (tiletype) {
                case TILETYPE_AIR:
                        t = asset_get(P("./sprites/sprite-0-13.dds"));
                        break;
                case TILETYPE_DIRT:
                case TILETYPE_DIRT_OVERHANG:
                case TILETYPE_SURFACE:
                        t = asset_get(P("./sprites/sprite-8-3.dds"));
                        break;
                case TILETYPE_DIRT_ROCK:
                        t = asset_get(P("./sprites/sprite-8-4.dds"));
                        break;
                case TILETYPE_GRASS:
                        t = asset_get(P("./sprites/sprite-11-4.dds"));
                        break;
                case TILETYPE_GRASS_ROCK1:
                case TILETYPE_GRASS_ROCK2:
                        t = asset_get(P("./sprites/sprite-11-5.dds"));
                        break;
                case TILETYPE_TREE:
                        t = asset_get(P("./sprites/sprite-12-3.dds"));
                        break;
                case TILETYPE_BRICK:
                        t = asset_get(P("./sprites/sprite-8-0.dds"));
                        break;
                case TILETYPE_DOOR:
                        t = asset_get(P("./sprites/sprite-8-12.dds"));
                        break;
                case TILETYPE_DOOR_OPEN:
                        t = asset_get(P("./sprites/sprite-8-13.dds"));
                        break;
                case TILETYPE_COBWEB:
                        t = asset_get(P("./sprites/sprite-11-1.dds"));
                        break;
                default:
                        t = asset_get(P("./sprites/sprite-0-13.dds"));
                        break;
        }
        return t;
}

float tile_get_transparency(int tiletype)
{
        switch (tiletype) {
                case TILETYPE_DOOR:
                case TILETYPE_DOOR_OPEN:
                        return 0.25;
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
                case '"':
                        return TILETYPE_GRASS;
                case '|':
                        return TILETYPE_TREE;
        }

        warning("Unknown tile type character: '%c'", c);
        return TILETYPE_NONE;
}

static int tile_counts[NUM_TILE_TYPES];

/* This just runs through the file and fills some vertex buffers with tile
 * properties */

static int SDL_RWreadline(SDL_RWops *file, char *buffer, int buffersize)
{

        char c;
        int status = 0;
        int i = 0;
        while (1) {

                status = SDL_RWread(file, &c, 1, 1);

                if (status == -1)
                        return -1;
                if (i == buffersize - 1)
                        return -1;
                if (status == 0)
                        break;

                buffer[i] = c;
                i++;

                if (c == '\n') {
                        buffer[i] = '\0';
                        return i;
                }
        }

        if (i > 0) {
                buffer[i] = '\0';
                return i;
        } else {
                return 0;
        }
}

level *level_load_file(const char *filename)
{
        int i;

        for (i = 0; i < NUM_TILE_TYPES; i++) {
                tile_counts[i] = 0;
        }

        level *l = malloc(sizeof(level));
        l->num_tile_sets = NUM_TILE_TYPES;
        l->tile_sets = malloc(sizeof(tile_set) * NUM_TILE_TYPES);
        l->tile_map = calloc(sizeof(int), MAX_WIDTH * MAX_HEIGHT);

        /* Load level name, placing null byte when encountering the . */
        const char *start = filename + strlen(filename);
        while (*start-- != '/')
                ; // Find the part past the last /
        strncpy(l->name, start + 2, LEVEL_NAME_LIMIT);
        l->name[LEVEL_NAME_LIMIT - 1] = '\0';
        for (i = 0; i < LEVEL_NAME_LIMIT; ++i)
                if (l->name[i] == '.') {
                        l->name[i] = '\0';
                        break;
                }

        SDL_RWops *file = SDL_RWFromFile(filename, "r");
        char line[MAX_WIDTH];

        int y = 0;
        int x = 0;
        while (SDL_RWreadline(file, line, 1024)) {

                for (x = 0; x < strlen(line); x++) {
                        char c = line[x];
                        int type = char_to_tile(c);

                        l->tile_map[x + y * MAX_WIDTH] = type;
                        tile_counts[type]++;
                }

                y++;
        }

        SDL_RWclose(file);

        /* Start from 1, type 0 is none! */
        for (int i = 1; i < NUM_TILE_TYPES; i++) {

                int num_tiles = tile_counts[i];

                float *position_data =
                    malloc(sizeof(float) * 3 * 4 * num_tiles);
                float *uv_data = malloc(sizeof(float) * 2 * 4 * num_tiles);

                int pos_i = 0;
                int uv_i = 0;

                for (x = 0; x < MAX_WIDTH; x++)
                        for (y = 0; y < MAX_HEIGHT; y++) {
                                int type = l->tile_map[x + y * MAX_WIDTH];
                                if (type == i) {
                                        position_data[pos_i] = x * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] = y * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] = 0;
                                        pos_i++;

                                        position_data[pos_i] =
                                            (x + 1) * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] = y * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] = 0;
                                        pos_i++;

                                        position_data[pos_i] =
                                            (x + 1) * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] =
                                            (y + 1) * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] = 0;
                                        pos_i++;

                                        position_data[pos_i] = x * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] =
                                            (y + 1) * TILE_SIZE;
                                        pos_i++;
                                        position_data[pos_i] = 0;
                                        pos_i++;

                                        uv_data[uv_i] = 0;
                                        uv_i++;
                                        uv_data[uv_i] = 0;
                                        uv_i++;

                                        uv_data[uv_i] = 1;
                                        uv_i++;
                                        uv_data[uv_i] = 0;
                                        uv_i++;

                                        uv_data[uv_i] = 1;
                                        uv_i++;
                                        uv_data[uv_i] = 1;
                                        uv_i++;

                                        uv_data[uv_i] = 0;
                                        uv_i++;
                                        uv_data[uv_i] = 1;
                                        uv_i++;
                                }
                        }

                l->tile_sets[i].num_tiles = num_tiles;

                glGenBuffers(1, &l->tile_sets[i].positions_buffer);
                glGenBuffers(1, &l->tile_sets[i].texcoords_buffer);

                glBindBuffer(GL_ARRAY_BUFFER, l->tile_sets[i].positions_buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * num_tiles,
                             position_data, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, l->tile_sets[i].texcoords_buffer);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 4 * num_tiles,
                             uv_data, GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, 0);

                free(position_data);
                free(uv_data);
        }


        // Set level color
        l->color = vec3_new(0.28, 0.48, 0.25);

        return l;
}

void level_delete(level *l)
{

        /* Start from 1 as 0 is none tile set */
        for (int i = 1; i < l->num_tile_sets; i++) {
                glDeleteBuffers(1, &l->tile_sets[i].positions_buffer);
                glDeleteBuffers(1, &l->tile_sets[i].texcoords_buffer);
        }

        free(l->tile_map);
        free(l->tile_sets);
        free(l);
}

/* Render background of level based on its color */
void level_render_background(level *l)
{

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, graphics_viewport_width(), 0, graphics_viewport_height(), -1,
                1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        //        glEnable(GL_TEXTURE_2D);

        //       char filename[LEVEL_NAME_LIMIT * 2] = "./backgrounds/";
        //      strncat(filename, l->name, LEVEL_NAME_LIMIT);
        //     strncat(filename, ".dds", 5);
        //    texture *background = asset_get_load(P(filename));
        //   glBindTexture(GL_TEXTURE_2D, texture_handle(background));

        glColor3f(l->color.x, l->color.y, l->color.z);
        glBegin(GL_QUADS);

        glVertex3f(0, graphics_viewport_height(), 0.0);
        glTexCoord2f(1, 0);
        glVertex3f(graphics_viewport_width(), graphics_viewport_height(), 0.0);
        glTexCoord2f(1, 1);
        glVertex3f(graphics_viewport_width(), 0, 0.0);
        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 0.0);
        glTexCoord2f(0, 0);

        glEnd();

        glDisable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
}

/* Renders each tileset in one go. Uses vertex buffers. */

void level_render_tiles(level *l, vec2 camera_position)
{

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(camera_position.x - graphics_viewport_width() / 2,
                camera_position.x + graphics_viewport_width() / 2,
                -camera_position.y + graphics_viewport_height() / 2,
                -camera_position.y - graphics_viewport_height() / 2, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        /* Start from 1, 0 is no tiles! */

        for (int i = 1; i < l->num_tile_sets; i++) {
                // Tile transparency
                glColor4f(1.0, 1.0, 1.0, tile_get_transparency(i));

                texture *tile_tex = tile_get_texture(i);
                glBindTexture(GL_TEXTURE_2D, texture_handle(tile_tex));
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_EDGE);

                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);

                glBindBuffer(GL_ARRAY_BUFFER, l->tile_sets[i].positions_buffer);
                glVertexPointer(3, GL_FLOAT, 0, (void *)0);

                glBindBuffer(GL_ARRAY_BUFFER, l->tile_sets[i].texcoords_buffer);
                glTexCoordPointer(2, GL_FLOAT, 0, (void *)0);

                glDrawArrays(GL_QUADS, 0, l->tile_sets[i].num_tiles * 4);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glDisableClientState(GL_VERTEX_ARRAY);
        }

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
}

int level_tile_at(level *l, vec2 position)
{

        int x = floor(position.x / TILE_SIZE);
        int y = floor(position.y / TILE_SIZE);

        assert(x >= 0);
        assert(y >= 0);
        assert(x < MAX_WIDTH);
        assert(y < MAX_HEIGHT);

        return l->tile_map[x + y * MAX_WIDTH];
}

vec2 level_tile_position(level *l, int x, int y)
{

        return vec2_new(x * TILE_SIZE, y * TILE_SIZE);
}
