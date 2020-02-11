#pragma once

/* Contains important game constants and methods */

#include "common.h"

#define GAME_STATE_RUNNING 1
#define GAME_STATE_PAUSED 2
#define GAME_STATE_GAMEOVER -1
#define GAME_STATE_STOP 0

#define GAME_TICK 10 // How many frames are in a game tick

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TILE_SIZE 16

void demo_init();
int demo_update();
void demo_render();
void demo_destroy();
