#ifndef platformer_h
#define platformer_h

#include "corange.h"

#define GAME_STATE_RUNNING 1
#define GAME_STATE_PAUSED 2
#define GAME_STATE_GAMEOVER -1
#define GAME_STATE_STOP 0

void platformer_init();
void platformer_event(SDL_Event event);
int platformer_update();
void platformer_render();
void platformer_finish();

#endif
