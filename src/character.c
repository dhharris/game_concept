#include "character.h"
#include "level.h"

character *character_new()
{
        character *c = malloc(sizeof(character));
        c->position = vec2_zero();
        c->facing_left = 0;
        c->health = 100;
        c->name = "Charles";
        return c;
}

void character_delete(character *c)
{
        free(c);
}

void character_update(character *c)
{
        // Update any character attributes
}


/* Renders a simple quad to the screen */
void character_render(character *c, vec2 camera_position)
{

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(camera_position.x - graphics_viewport_width() / 2,
                camera_position.x + graphics_viewport_width() / 2,
                -camera_position.y + graphics_viewport_height() / 2,
                -camera_position.y - graphics_viewport_height() / 2, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);

        texture *character_tex = asset_get(P("./sprites/sprite-0-0.dds"));
        glBindTexture(GL_TEXTURE_2D, texture_handle(character_tex));

        /* Swaps the direction of the uvs when facing the opposite direction */
        if (c->facing_left) {
                glBegin(GL_TRIANGLES);
                glTexCoord2f(1, 1);
                glVertex3f(c->position.x, c->position.y + TILE_SIZE, 0);
                glTexCoord2f(1, 0);
                glVertex3f(c->position.x, c->position.y, 0);
                glTexCoord2f(0, 0);
                glVertex3f(c->position.x + TILE_SIZE, c->position.y, 0);

                glTexCoord2f(1, 1);
                glVertex3f(c->position.x, c->position.y + TILE_SIZE, 0);
                glTexCoord2f(0, 1);
                glVertex3f(c->position.x + TILE_SIZE, c->position.y + TILE_SIZE, 0);
                glTexCoord2f(0, 0);
                glVertex3f(c->position.x + TILE_SIZE, c->position.y, 0);
                glEnd();

        } else {

                glBegin(GL_TRIANGLES);
                glTexCoord2f(0, 1);
                glVertex3f(c->position.x, c->position.y + TILE_SIZE, 0);
                glTexCoord2f(0, 0);
                glVertex3f(c->position.x, c->position.y, 0);
                glTexCoord2f(1, 0);
                glVertex3f(c->position.x + TILE_SIZE, c->position.y, 0);

                glTexCoord2f(0, 1);
                glVertex3f(c->position.x, c->position.y + TILE_SIZE, 0);
                glTexCoord2f(1, 1);
                glVertex3f(c->position.x + TILE_SIZE, c->position.y + TILE_SIZE, 0);
                glTexCoord2f(1, 0);
                glVertex3f(c->position.x + TILE_SIZE, c->position.y, 0);
                glEnd();
        }

        glDisable(GL_TEXTURE_2D);

        glDisable(GL_BLEND);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
}
