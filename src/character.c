#include "character.h"
#include "helpers.h"
#include "level.h"
#include "raymath.h"
#include <stdlib.h>

character *character_new()
{
        character *c = malloc(sizeof(character));
        c->position = Vector2Zero();
        c->facing_left = 0;
        c->health = 99;
        c->name = "Steve";
        c->spritesheet = LoadImage("sprites/H37_gnome.bmp");
        c->sprite_index = Vector2Zero();
        Image tmp = get_sprite_from_sheet(c->spritesheet, c->sprite_index);
        c->texture = LoadTextureFromImage(tmp);
        UnloadImage(tmp);
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
