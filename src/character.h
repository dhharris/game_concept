#pragma once

#include "common.h"
#include "item.h"

/* character constants */
#define INVENTORY_SIZE 10

typedef struct {
        char *name;
        Vector2 position;
        Vector2 new_position;
        int facing_left; // Determine whether we need to invert the texture
        int health;
        item inventory[INVENTORY_SIZE];

        // Image and texture data
        Image spritesheet;
        Vector2 sprite_index;
        Texture2D texture;
} character;

character *character_new();
void character_destroy(character *c);

void character_update(character *c);
void character_render(character *c, Vector2 camera_position);
