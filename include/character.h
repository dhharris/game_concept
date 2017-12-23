#ifndef character_h
#define character_h

#include "corange.h"
#include "item.h"

/* important constants */
#define INVENTORY_SIZE 10

typedef struct {
        char *name;
        vec2 position;
        int facing_left; // Determine whether we need to invert the texture
        int health;
        item inventory[INVENTORY_SIZE];
} character;

character *character_new();
void character_destroy(character *c);

void character_update(character *c);
void character_render(character *c, vec2 camera_position);

#endif
