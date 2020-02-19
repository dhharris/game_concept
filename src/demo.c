#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#include "character.h"
#include "gen.h"
#include "level.h"

#include "demo.h"

// Some game state variables
static level *current_level = NULL;
static Vector2 camera_position = {0, 0};
static float level_time = 0;
static character *player = NULL;
static Camera2D camera = {0};

static int level_counter;

static void reset_level()
{
        // Set the level according to the level counter
        char path[LEVEL_NAME_LIMIT];
        level_get_path(path, level_counter);
        current_level = level_load_file(path);
        level_time = 0.0;

        player->position = player->new_position =
            current_level->character_position;
}

void demo_init(char *name)
{
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, name);
        SetTargetFPS(FPS);

        // generate new levels
        for (int i = 0; i < NUM_LEVELS; ++i) gen_level();

        // Create our main character
        player = character_new();

        // Initialize camera
        camera.target = player->position;
        // Center of the screen
        camera.offset = (Vector2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        // Reset all the game variables for the current level
        reset_level();
}

// Update game logic. Returns the status of the game state
int demo_update()
{
        // Update player location
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                // TODO: Fix mouse movement
                // player->new_position = GetMousePosition();
        }
        // Keyboard movement
        if (IsKeyPressed(KEY_UP))
                player->new_position.y -= TILE_SIZE;
        else if (IsKeyPressed(KEY_DOWN))
                player->new_position.y += TILE_SIZE;
        else if (IsKeyPressed(KEY_LEFT))
                player->new_position.x -= TILE_SIZE;
        else if (IsKeyPressed(KEY_RIGHT))
                player->new_position.x += TILE_SIZE;
        else if (IsKeyPressed(KEY_R))
                player->new_position = (Vector2){0, 0};

        character_update(player);

        // Update items logic
        //        item_map_update(current_level->item_map);

        // Camera follows player
        camera.target = player->position;

        level_time += (float) 1 / FPS;

        // Check character health. If he dies then end the game
        if (player->health <= 0)
                return GAME_STATE_GAMEOVER;
        return GAME_STATE_RUNNING;
}

void gameover_render()
{
        // Clear the screen to a single color
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

        level_render_background(current_level);
        level_render_tiles(current_level);
        // item_map_render(current_level->item_map, camera_position);

        // Draw the player
        DrawTextureV(player->texture, player->position, WHITE);

        EndMode2D();

        // Update character ui elements
        char health_label[10];
        snprintf(health_label, 10, "Health %2d", player->health);
        DrawText(health_label, 10, 10, 20, DARKGRAY);

        char time_label[10];
        snprintf(time_label, 10, "Time %2.2f", level_time);
        DrawText(time_label, WINDOW_WIDTH / 2 - 20, 10, 20, DARKGRAY);

        DrawFPS(WINDOW_WIDTH - 85, 10);

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
