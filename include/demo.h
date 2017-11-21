#ifndef demo_h
#define demo_h

/* Contains important game constants and methods */

#include "corange.h"

#define GAME_STATE_RUNNING 1
#define GAME_STATE_PAUSED 2
#define GAME_STATE_GAMEOVER -1
#define GAME_STATE_STOP 0

#define GAME_TICK 10 // How many frames are in a game tick

void demo_init();
void demo_event(SDL_Event event);
int demo_update();
void demo_render();
void demo_finish();

#endif
