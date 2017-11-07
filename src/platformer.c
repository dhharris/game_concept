#include <math.h>
#include <stdio.h>

#include "corange.h"

#include "character.h"
#include "coin.h"
#include "level.h"

#include "platformer.h"

/* Some game state variables */
static level *current_level = NULL;
static vec2 camera_position = {0, 0};
static float level_time = 0;

/* We store all the coin positions here */
enum
{
        COIN_COUNT = 45,
};

static vec2 coin_positions[COIN_COUNT] = {
    {16, 23}, {33, 28}, {41, 22}, {20, 19},  {18, 28},  {36, 20},  {20, 30},
    {31, 18}, {45, 23}, {49, 26}, {25, 18},  {20, 37},  {44, 32},  {66, 20},
    {52, 20}, {63, 11}, {52, 12}, {39, 13},  {27, 11},  {73, 20},  {65, 29},
    {72, 29}, {78, 30}, {78, 20}, {83, 22},  {87, 22},  {90, 24},  {94, 19},
    {99, 18}, {82, 13}, {79, 14}, {106, 22}, {102, 30}, {100, 35}, {93, 27},
    {88, 34}, {98, 40}, {96, 40}, {94, 40},  {86, 40},  {81, 37},  {77, 38},
    {72, 34}, {65, 38}, {71, 37}};

static void reset_game()
{

        /* Set the starting level to demo.level */
        current_level = asset_get(P("./levels/demo.level"));
        level_time = 0.0;

        /* New main character entity */
        character *main_char = entity_get("main_char");
        main_char->position = vec2_mul(vec2_new(20, 20), TILE_SIZE);
        main_char->velocity = vec2_zero();

        /* We can create multiple entities using a name format string like
         * printf */
        entities_new("coin_id_%i", COIN_COUNT, coin);

        /* Get an array of pointers to all coin entities */
        coin *coins[COIN_COUNT];
        entities_get(coins, NULL, coin);

        /* Set all the coin initial positions */
        for (int i = 0; i < COIN_COUNT; i++) {
                coins[i]->position = vec2_mul(coin_positions[i], TILE_SIZE);
        }

        /* Deactivate victory and new game UI elements */
        ui_button *victory = ui_elem_get("victory");
        ui_button *new_game = ui_elem_get("new_game");

        victory->active = 0;
        new_game->active = 0;
}

void on_newgame(ui_button *b, void *unused)
{
        reset_game();
}

void platformer_init()
{

        /* Set the viewport title */
        graphics_viewport_set_title("Platformer");

        /* Register functions for loading/unloading files with the extension
         * .level */
        asset_handler(level, "level", level_load_file, level_delete);

        /* Load Assets */
        folder_load(P("./tiles/"));
        folder_load(P("./backgrounds/"));
        folder_load(P("./sounds/"));
        folder_load(P("./levels/"));

        /* Register some handlers for creating and destroying entity types */
        entity_handler(character, character_new, character_delete);
        entity_handler(coin, coin_new, coin_delete);

        /* Create our main character */
        character *main_char = entity_new("main_char", character);

        /* Add some UI elements */
        ui_button *framerate = ui_elem_new("framerate", ui_button);
        ui_button_move(framerate, vec2_new(10, 10));
        ui_button_resize(framerate, vec2_new(30, 25));
        ui_button_set_label(framerate, " ");
        ui_button_disable(framerate);

        ui_button *health = ui_elem_new("health", ui_button);
        ui_button_move(health, vec2_new(50, 10));
        ui_button_resize(health, vec2_new(120, 25));
        ui_button_set_label(health, "Health 0");
        ui_button_disable(health);

        ui_button *time = ui_elem_new("time", ui_button);
        ui_button_move(time, vec2_new(180, 10));
        ui_button_resize(time, vec2_new(110, 25));
        ui_button_set_label(time, "Time 000000");
        ui_button_disable(time);

        ui_button *victory = ui_elem_new("victory", ui_button);
        ui_button_move(victory, vec2_new(365, 200));
        ui_button_resize(victory, vec2_new(70, 25));
        ui_button_set_label(victory, "Victory!");
        ui_button_disable(victory);

        ui_button *new_game_but = ui_elem_new("new_game", ui_button);
        ui_button_move(new_game_but, vec2_new(365, 230));
        ui_button_resize(new_game_but, vec2_new(70, 25));
        ui_button_set_label(new_game_but, "New Game");

        ui_button_set_onclick(new_game_but, on_newgame);

        /* Reset all the game variables */
        reset_game();
}

/* Some flags to monitor key inputs */
static int left_held = 0;
static int right_held = 0;
static int up_held = 0;
static int down_held = 0;

void platformer_event(SDL_Event event)
{

        switch (event.type) {
        case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_LEFT) {
                        left_held = 1;
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                        right_held = 1;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                        up_held = 1;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                        down_held = 1;
                }
                if (left_held || right_held || up_held || down_held) {
                        character *main_char = entity_get("main_char");
                        main_char->walk_timer += 0.15;
                }

                break;

        case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_LEFT) {
                        left_held = 0;
                }
                if (event.key.keysym.sym == SDLK_RIGHT) {
                        right_held = 0;
                }
                if (event.key.keysym.sym == SDLK_UP) {
                        up_held = 0;
                }
                if (event.key.keysym.sym == SDLK_DOWN) {
                        down_held = 0;
                }
                break;
        }
}

static void collision_detection()
{

        /*
           Collision is fairly simplistic and looks something like this.

           @-----@    We check for collision in those points here which
           @       @   are @ signs. If any are colliding with a solid tile
           |       |   then we shift the player so that they are no longer
           @       @   colliding with it. Also invert the velocity.
           @-----@
           */

        character *main_char = entity_get("main_char");

        const float buffer = 4;
        const float bounce = 0.5;

        vec2 diff;

        /* Bottom Collision */

        diff = vec2_fmod(main_char->position, TILE_SIZE);

        vec2 bottom1 =
            vec2_add(main_char->position, vec2_new(buffer, TILE_SIZE));
        vec2 bottom2 = vec2_add(main_char->position,
                                vec2_new(TILE_SIZE - buffer, TILE_SIZE));

        int bottom1_col =
            tile_has_collision(level_tile_at(current_level, bottom1));
        int bottom2_col =
            tile_has_collision(level_tile_at(current_level, bottom2));

        if (bottom1_col || bottom2_col) {
                main_char->position =
                    vec2_add(main_char->position, vec2_new(0, -diff.y));
                main_char->velocity.y *= -bounce;
        }

        /* Top Collision */

        diff = vec2_fmod(main_char->position, TILE_SIZE);

        vec2 top1 = vec2_add(main_char->position, vec2_new(buffer, 0));
        vec2 top2 =
            vec2_add(main_char->position, vec2_new(TILE_SIZE - buffer, 0));

        int top1_col = tile_has_collision(level_tile_at(current_level, top1));
        int top2_col = tile_has_collision(level_tile_at(current_level, top2));

        if (top1_col || top2_col) {
                main_char->position = vec2_add(main_char->position,
                                               vec2_new(0, TILE_SIZE - diff.y));
                main_char->velocity.y *= -bounce;
        }

        /* Left Collision */

        diff = vec2_fmod(main_char->position, TILE_SIZE);

        vec2 left1 = vec2_add(main_char->position, vec2_new(0, buffer));
        vec2 left2 =
            vec2_add(main_char->position, vec2_new(0, TILE_SIZE - buffer));

        int left1_col = tile_has_collision(level_tile_at(current_level, left1));
        int left2_col = tile_has_collision(level_tile_at(current_level, left2));

        if (left1_col || left2_col) {
                main_char->position = vec2_add(main_char->position,
                                               vec2_new(TILE_SIZE - diff.x, 0));
                main_char->velocity.x *= -bounce;
        }

        /* Right Collision */

        diff = vec2_fmod(main_char->position, TILE_SIZE);

        vec2 right1 =
            vec2_add(main_char->position, vec2_new(TILE_SIZE, buffer));
        vec2 right2 = vec2_add(main_char->position,
                               vec2_new(TILE_SIZE, TILE_SIZE - buffer));

        int right1_col =
            tile_has_collision(level_tile_at(current_level, right1));
        int right2_col =
            tile_has_collision(level_tile_at(current_level, right2));

        if (right1_col || right2_col) {
                main_char->position =
                    vec2_add(main_char->position, vec2_new(-diff.x, 0));
                main_char->velocity.x *= -bounce;
        }
}

static void collision_detection_coins()
{

        /* We simply check if the player intersects with the coins */

        character *main_char = entity_get("main_char");

        vec2 top_left =
            vec2_add(main_char->position, vec2_new(-TILE_SIZE, -TILE_SIZE));
        vec2 bottom_right =
            vec2_add(main_char->position, vec2_new(TILE_SIZE, TILE_SIZE));

        /* Again we collect pointers to all the coin type entities */
        int num_coins = 0;
        coin *coins[COIN_COUNT];
        entities_get(coins, &num_coins, coin);

        for (int i = 0; i < num_coins; i++) {
                /* Check if they are within the main char bounding box */
                if ((coins[i]->position.x > top_left.x) &&
                    (coins[i]->position.x < bottom_right.x) &&
                    (coins[i]->position.y > top_left.y) &&
                    (coins[i]->position.y < bottom_right.y)) {

                        /* Remove them from the entity manager and delete */
                        char *coin_name = entity_name(coins[i]);
                        entity_delete(coin_name);

                        /* Play a hurting sound */
                        audio_sound_play(
                            asset_get_as(P("./sounds/hurt.wav"), sound), 0);

                        /* Take away from player's health */
                        main_char->health -= 10;

                        /* Update the ui text */
                        ui_button *health = ui_elem_get("health");
                        sprintf(health->label->string, "Health %d",
                                main_char->health);
                        ui_text_draw(health->label);
                }
        }

        ui_button *victory = ui_elem_get("victory");

        /* if all the coins are gone and the victory rectangle isn't disaplayed
         * then show it */
        if ((entity_type_count(coin) == 0) && (!victory->active)) {
                ui_button *victory = ui_elem_get("victory");
                ui_button *new_game = ui_elem_get("new_game");
                victory->active = 1;
                new_game->active = 1;
        }
}

/* Update game logic. Returns the status of the game state */
int platformer_update()
{

        character *main_char = entity_get("main_char");

        if (left_held) {
                main_char->velocity.x -= 0.1;
                main_char->facing_left = 1;
        } else if (right_held) {
                main_char->velocity.x += 0.1;
                main_char->facing_left = 0;
        } else {
                main_char->velocity.x *= 0.95;
        }

        if (up_held) {
                main_char->velocity.y -= 0.1;
        } else if (down_held) {
                main_char->velocity.y += 0.1;
        } else {
                main_char->velocity.y *= 0.95;
        }

        /* Update moves position based on velocity */
        character_update(main_char);

        /* Two phases of collision detection */
        collision_detection();
        collision_detection_coins();

        /* Camera follows main character */
        camera_position =
            vec2_new(main_char->position.x, -main_char->position.y);

        /* Update the framerate text */
        ui_button *framerate = ui_elem_get("framerate");
        ui_button_set_label(framerate, frame_rate_string());

        /* Update the time text */
        ui_button *victory = ui_elem_get("victory");
        if (!victory->active) {
                level_time += frame_time();
                ui_button *time = ui_elem_get("time");
                sprintf(time->label->string, "Time %06i", (int)level_time);
                ui_text_draw(time->label);
        }
        /* Check character health. If he dies then end the game */
        if (main_char->health <= 0)
                return GAME_STATE_GAMEOVER;
        return GAME_STATE_RUNNING;
}

void gameover_render()
{
        level l;
        strncpy(l.name, "gameover", 9);
        /* Clear the screen to a single color */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        level_render_background(&l);
}

void pause_render()
{
        // TODO: Implement me!
}

void platformer_render()
{

        /* Clear the screen to a single color */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level_render_background(current_level);

        character_render(entity_get_as("main_char", character),
                         camera_position);

        /* Get pointers to all the coins for rendering */
        coin *coins[COIN_COUNT];
        int num_coins = 0;
        entities_get(coins, &num_coins, coin);

        for (int i = 0; i < num_coins; i++) {
                coin_render(coins[i], camera_position);
        }

        level_render_tiles(current_level, camera_position);
}

void platformer_finish()
{
        /* Entity and asset managers will automatically delete any remaining
         * objects. */
}

int main(int argc, char **argv)
{

#ifdef _WIN32
        FILE *ctt = fopen("CON", "w");
        FILE *fout = freopen("CON", "w", stdout);
        FILE *ferr = freopen("CON", "w", stderr);
#endif

        /* Init Corange, pointing to the assets_core folder */
        corange_init("./assets_core");
        graphics_viewport_set_title("A Game");
        graphics_viewport_set_size(800, 600);

        platformer_init();

        /* Set the game state, create SDL_Event struct to monitor events */
        int state = GAME_STATE_RUNNING;
        SDL_Event event;

        while (state) {

                /* Frame functions used to monitor frame times, FPS and other */
                frame_begin();

                while (SDL_PollEvent(&event)) {

                        switch (event.type) {
                        case SDL_KEYUP:
                                /* Pause on ESCAPE and Screenshot on print
                                 * screen
                                 */
                                if (event.key.keysym.sym == SDLK_ESCAPE) {
                                        if (state == GAME_STATE_RUNNING)
                                                state = GAME_STATE_PAUSED;
                                        else
                                                state = GAME_STATE_RUNNING;
                                }
                                if (event.key.keysym.sym == SDLK_PRINTSCREEN) {
                                        graphics_viewport_screenshot();
                                }
                                break;
                        case SDL_QUIT:
                                /* A quitting event such as pressing cross in
                                 * top right corner */
                                state = GAME_STATE_STOP;
                                break;
                        }

                        /* Also send this event off to the game and ui */
                        platformer_event(event);
                        ui_event(event);
                }
                /* Only update game if it is running */
                if (state == GAME_STATE_RUNNING) {
                        state = platformer_update();
                        ui_update();

                        platformer_render();
                        ui_render();

                } else if (state == GAME_STATE_PAUSED) {
                        /* Render pause menu */
                        pause_render();
                } else if (state == GAME_STATE_GAMEOVER) {
                        /* Render Game Over screen */
                        gameover_render();
                }

                /* Flip the Screen Buffer. We've finished with this
                 * frame. */
                graphics_swap();

                /* This allows us to fix the framerate to 60 fps, even on my
                 * laptop with vsync broken */
                frame_end();
        }

        platformer_finish();

        /* Corange will unload remaining assets and delete any remaining
         * entities */
        corange_finish();

        return 0;
}
