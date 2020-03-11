#include "character.h"
#include "helpers.h"
#include "level.h"
#include "raymath.h"
#include <stdlib.h>

// These variables control player speed
#define ONE_IN 6
static int FRAME_COUNTER = 0;

void flip_character_sprite(character *c)
{
        Image flipped = GetTextureData(c->texture);
        ImageFlipHorizontal(&flipped);
        UnloadTexture(c->texture);
        LoadTextureFromImage(flipped);
        UnloadImage(flipped);
        c->facing_left = !c->facing_left;
}

character *character_new()
{
        character *c = malloc(sizeof(character));
        c->position = Vector2Zero();
        c->new_position = Vector2Zero();
        c->facing_left = 0;
        c->health = 99;
        c->name = "Steve";
        // Sprite fields
        c->spritesheet = LoadImage("sprites/sprite-0-2.png");
        c->sprite_index = Vector2Zero();
        Image tmp = get_sprite_from_sheet(c->spritesheet, c->sprite_index);
        c->texture = LoadTextureFromImage(tmp);
        UnloadImage(tmp);
        return c;
}

void character_destroy(character *c)
{
        UnloadImage(c->spritesheet);
        UnloadTexture(c->texture);
        free(c);
}

// Update any character attributes
void character_update(character *c)
{
        // Flip character image if necessary
        if (c->new_position.x < c->position.x && c->facing_left == 0) {
                flip_character_sprite(c);
        }
        if (c->new_position.x > c->position.x && c->facing_left == 1) {
                flip_character_sprite(c);
        }
        if (FRAME_COUNTER % ONE_IN == 0) {
                // Move the character
                // TODO: For mouse movement, make player one tile closer
                if (!Vector2Eq(c->position, c->new_position)) {
                        TraceLog(LOG_INFO, "[%s] Character moving to (%f, %f)",
                                 __FILE__, c->new_position.x,
                                 c->new_position.y);
                }
                c->position = c->new_position;
                FRAME_COUNTER = 0;
        } else {
                // Reset the new position
                c->new_position = c->position;
        }
        FRAME_COUNTER++;
}
