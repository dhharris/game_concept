#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

#include "character.h"
#include "gen.h"
#include "level.h"

#include "demo.h"

/* Some game state variables */
static level *current_level = NULL;
static Vector2 camera_position = {0, 0};
static float level_time = 0;
static character *player = NULL;
static Camera2D camera = { 0 };

int frame_counter;
static int level_counter;

static void reset_level()
{
        /* Set the level according to the level counter */
        char path[LEVEL_NAME_LIMIT];
        level_get_path(path, level_counter);
//        current_level = asset_get(P(path));
        level_time = 0.0;

        player->position = current_level->character_position;
}

void demo_init()
{
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "demo");


        /* generate new levels */
        // int i;
        // for (i = 0; i < NUM_LEVELS; ++i) gen_level();

        /* Create our main character */
        player = character_new();

        /* Reset all the game variables */
        reset_level();

        /* Initialize camera */
        camera.target = player->position;
        camera.offset = (Vector2){ WINDOW_WIDTH/2, WINDOW_HEIGHT/2 };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
}

/* Update game logic. Returns the status of the game state */
int demo_update()
{
        character_update(player);

        /* Update items logic */
        item_map_update(current_level->item_map);

        /* Camera follows player */
        camera.target = player->position;

        /* Check character health. If he dies then end the game */
        if (player->health <= 0)
                return GAME_STATE_GAMEOVER;
        return GAME_STATE_RUNNING;

}

void gameover_render()
{
        /* Clear the screen to a single color */
        ClearBackground(RAYWHITE);
}

void pause()
{
}

void unpause()
{
}

void demo_render()
{
        BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        // level_render_background(current_level);
        // level_render_tiles(current_level, camera_position);
        // item_map_render(current_level->item_map, camera_position);

        /* Draw the player */
        DrawTexture(
                player->texture, player->position.x, player->position.y, RED);

        EndMode2D();

        /* Update character ui elements */
        char health_label[11];
        snprintf(health_label, 11, "Health %d", player->health);
        DrawText(health_label, 190, 200, 20, LIGHTGRAY);


        EndDrawing();
}

void demo_destroy()
{
        character_destroy(player);
        int i;
        char path[LEVEL_NAME_LIMIT];

        for (i = 0; i < NUM_LEVELS; ++i) {
                level_get_path(path, i);
                remove(path);
        }
}
