#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "corange.h"

#include "character.h"
#include "gen.h"
#include "level.h"

#include "demo.h"

/* Some game state variables */
static level *current_level = NULL;
static vec2 camera_position = {0, 0};
static float level_time = 0;

int frame_counter;
static int level_counter;

static void reset_game()
{
        /* Set the level according to the level counter */
        char path[LEVEL_NAME_LIMIT];
        level_get_path(path, level_counter);
        current_level = asset_get(P(path));
        level_time = 0.0;

        /* New main character entity */
        character *main_char = entity_get("main_char");
        main_char->position = current_level->character_position;

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

void demo_init()
{
        /* Set the viewport title */
        graphics_viewport_set_title("demo");

        /* Register functions for loading/unloading files with the extension
         * .level */
        asset_handler(level, "level", level_load_file, level_destroy);

        /* generate new levels */
        int i;
        for (i = 0; i < NUM_LEVELS; ++i) gen_level();

        /* Load Assets */
        folder_load(P("./sprites/"));
        folder_load(P("./backgrounds/"));
        folder_load(P("./sounds/"));
        folder_load(P("./levels/"));

        /* Register some handlers for creating and destroying entity types */
        entity_handler(character, character_new, character_destroy);

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
        ui_button_set_label(health, "Health 100");
        ui_button_disable(health);

        ui_button *time_but = ui_elem_new("time", ui_button);
        ui_button_move(time_but, vec2_new(180, 10));
        ui_button_resize(time_but, vec2_new(110, 25));
        ui_button_set_label(time_but, "time 000000");
        ui_button_disable(time_but);

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

        /* Seed random numbers */
        srand(time(NULL));

        /* Reset all the game variables */
        reset_game();
}

/* Some flags to monitor key inputs */
static int left_held = 0;
static int right_held = 0;
static int up_held = 0;
static int down_held = 0;

void demo_event(SDL_Event event)
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

void move_character(character *c)
{
        vec2 prev = vec2_new(c->position.x, c->position.y);

        if (left_held) {
                c->position.x -= TILE_SIZE;
                c->facing_left = 1;
        } else if (right_held) {
                c->position.x += TILE_SIZE;
                c->facing_left = 0;
        } else if (up_held) {
                c->position.y -= TILE_SIZE;
        } else if (down_held) {
                c->position.y += TILE_SIZE;
        }

        if (tile_has_collision(level_tile_at(current_level, c->position))) {
                // Move character back to previous position and print a
                // nice message
                c->position = prev;
                debug("%s\n", "Cannot move that way");
        }

        // testing for items
        if (!vec2_equ(c->position, prev)) {
                item_stack **item_map = current_level->item_map;
                item_map_add_item(item_map, ITEMTYPE_NONE, prev);
                item_stack *head = item_map_stack_at(item_map, c->position);
                debug("I am standing on %d items", item_stack_count(head));
        }

        current_level->character_position = c->position;
}

/* Update game logic. Returns the status of the game state */
int demo_update()
{
        character *main_char = entity_get("main_char");
        if (++frame_counter == GAME_TICK) {
                move_character(main_char);
                frame_counter = 0;
        }
        character_update(main_char);

        /* Update character ui elements */
        ui_button *health = ui_elem_get("health");
        sprintf(health->label->string, "Health %d", main_char->health);
        ui_text_draw(health->label);

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

        /* Update items logic */
        item_map_update(current_level->item_map);

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

void pause()
{
        /* Show new game button */
        ui_button *new_game = ui_elem_get("new_game");
        new_game->active = 1;
}

void unpause()
{
        /* Hide new game button */
        ui_button *new_game = ui_elem_get("new_game");
        new_game->active = 0;
}

void demo_render()
{
        /* Clear the screen to a single color */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level_render_background(current_level);
        level_render_tiles(current_level, camera_position);
        item_map_render(current_level->item_map, camera_position);
        character_render(entity_get_as("main_char", character),
                         camera_position);
}

void demo_finish()
{
        /* Entity and asset managers will automatically free any remaining
         * objects. */
        int i;
        char path[LEVEL_NAME_LIMIT];;

        for (i = 0; i < NUM_LEVELS; ++i) {
                level_get_path(path, i);
                remove(path);
        }

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

        demo_init();

        /* Set the game state, create SDL_Event struct to monitor events */
        int state = GAME_STATE_RUNNING;
        SDL_Event event;

        while (state) {
                /* Frame functions used to monitor frame times, FPS and other */
                frame_begin();

                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                                case SDL_KEYUP:
                                        /* Pause on ESCAPE and Screenshot on
                                         * print screen
                                         */
                                        if (event.key.keysym.sym ==
                                            SDLK_ESCAPE) {
                                                if (state ==
                                                    GAME_STATE_RUNNING) {
                                                        pause();
                                                        state =
                                                            GAME_STATE_PAUSED;
                                                } else {
                                                        unpause();
                                                        state =
                                                            GAME_STATE_RUNNING;
                                                }
                                        }
                                        if (event.key.keysym.sym ==
                                            SDLK_PRINTSCREEN) {
                                                graphics_viewport_screenshot();
                                        }
                                        if (event.key.keysym.sym == SDLK_0) {
                                                level_counter = 0;
                                                reset_game();
                                        }
                                        if (event.key.keysym.sym == SDLK_1) {
                                                level_counter = 1;
                                                reset_game();
                                        }
                                        if (event.key.keysym.sym == SDLK_2) {
                                                level_counter = 2;
                                                reset_game();
                                        }
                                        break;
                                case SDL_QUIT:
                                        /* A quitting event such as pressing
                                         * cross in top right corner */
                                        state = GAME_STATE_STOP;
                                        break;
                        }

                        /* Also send this event off to the game and ui */
                        demo_event(event);
                        ui_event(event);
                }
                /* Only update game if it is running */
                if (state == GAME_STATE_RUNNING) {
                        state = demo_update();
                        ui_update();
                        demo_render();
                        ui_render();
                } else if (state == GAME_STATE_PAUSED) {
                        /* Render pause menu */
                        ui_update();
                        ui_render();
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

        demo_finish();

        /* Corange will unload remaining assets and free any remaining
         * entities */
        corange_finish();

        return 0;
}
