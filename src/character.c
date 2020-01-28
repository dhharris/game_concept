#include <stdlib.h>
#include "character.h"
#include "level.h"
#include "raymath.h"

character *character_new()
{
        character *c = malloc(sizeof(character));
        c->position = Vector2Zero();
        c->facing_left = 0;
        c->health = 100;
        c->name = "Charles";
        c->texture = LoadTexture("sprites/character.png");
        return c;
}

void character_destroy(character *c)
{
        free(c);
}

void character_update(character *c)
{
        // Update any character attributes
}
