#pragma once

/* Contains important game constants and methods */

#define GAME_STATE_RUNNING 1
#define GAME_STATE_PAUSED 2
#define GAME_STATE_GAMEOVER -1
#define GAME_STATE_STOP 0

#define GAME_TICK 10 // How many frames are in a game tick

void demo_init();
int demo_update();
void demo_render();
void demo_destroy();
