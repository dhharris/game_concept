#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#include "character.h"
#include "gen.h"
#include "helpers.h"
#include "level.h"

#include "demo.h"

// Some game state variables
static character *player;
static level *current_level;
static float level_time;
static Camera2D camera;

static void reset_level()
{
        level_destroy(current_level);
        current_level = level_load();
        level_time = 0.0;

        player->position = player->new_position =
            current_level->starting_position;
}

void demo_init(char *name)
{
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, name);
        SetTargetFPS(FPS);

        // generate new levels
        // TODO: Move this to gen.c?
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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Transform mouse position so it is relative to the map
                Vector2 mouse_pos =
                    Vector2Add(GetMousePosition(),
                               Vector2Subtract(camera.target, camera.offset));
                // Snap to grid
                Vector2 coordinates =
                    level_get_position(current_level, mouse_pos);
                Vector2 new_position = Vector2Scale(coordinates, TILE_SIZE);

                // Free path data if it exists
                path_destroy(player->path);
                player->path = level_shortest_path(current_level, player->position, new_position);
        }
        // Keyboard movement
        if (IsKeyDown(KEY_UP)) {
                player->new_position.y -= TILE_SIZE;
        } else if (IsKeyDown(KEY_DOWN)) {
                player->new_position.y += TILE_SIZE;
        } else if (IsKeyDown(KEY_LEFT)) {
                player->new_position.x -= TILE_SIZE;
        } else if (IsKeyDown(KEY_RIGHT)) {
                player->new_position.x += TILE_SIZE;
        } else if (IsKeyPressed(KEY_R)) {
                player->new_position = current_level->starting_position;
                level_time = 0.0;
        } else if (IsKeyPressed(KEY_E)) { // Perform player actions
                // Are we performing an action that causes level reset?
                // stairs, teleports, etc.
                if (level_should_reset(current_level, player->position)) {
                        reset_level();
                }
        } else if (IsKeyPressed(KEY_Q)) { // Drop an item
                item_map_add_item(current_level->item_map, ITEMTYPE_COINS,
                                  player->position);
        }

        // Move player to previous position if they are trying to access an
        // invalid position
        if (!level_validate_position(current_level, player->new_position)) {
                player->new_position = player->position;
        }
        character_update(player);

        // Update items logic
        item_map_update(current_level->item_map);

        // Camera follows player
        camera.target = player->position;

        level_time += (float)1 / FPS;

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
        item_map_render(current_level->item_map);

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

        char player_pos_label[25];
        Vector2 level_pos = level_get_position(current_level, player->position);
        snprintf(player_pos_label, 25, "Player X: %2.2f Y: %2.2f", level_pos.x,
                 level_pos.y);
        DrawText(player_pos_label, 100, 132, 16, BLACK);

        DrawFPS(WINDOW_WIDTH - 85, 10);

        EndDrawing();
}

void demo_destroy()
{
        character_destroy(player);
        int i;
        char path[LEVEL_NAME_LIMIT];

        /* Remove level files */
        for (i = 0; i < NUM_LEVELS; ++i) {
                level_get_path(path, i);
                remove(path);
        }
}
