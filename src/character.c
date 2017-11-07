#include "character.h"

character *character_new()
{
        character *c = malloc(sizeof(character));
        c->position = vec2_zero();
        c->velocity = vec2_zero();
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
        c->velocity.x = clamp(c->velocity.x, -5.0, 5.0); // Limit speed to 5
        c->position = vec2_add(c->position, c->velocity);
}

static float previous_x = 0.0;

/* Whether the character is airborne, i.e. jumping or falling */
int is_airborne(character *c)
{
        return c->velocity.y != 0;
}

int is_moving(character *c)
{
        return fabs(c->velocity.x) > 0.5;
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

        /* Conditional as to if we render walking or normal icon */
        texture *character_tex;
        if (is_moving(c)) {
                character_tex = asset_get(P("./tiles/character_walk.dds"));
        } else {
                character_tex = asset_get(P("./tiles/character.dds"));
        }
        glBindTexture(GL_TEXTURE_2D, texture_handle(character_tex));

        /* Swaps the direction of the uvs when facing the opposite direction */
        if (c->facing_left) {
                glBegin(GL_TRIANGLES);
                glTexCoord2f(1, 1);
                glVertex3f(c->position.x, c->position.y + 32, 0);
                glTexCoord2f(1, 0);
                glVertex3f(c->position.x, c->position.y, 0);
                glTexCoord2f(0, 0);
                glVertex3f(c->position.x + 32, c->position.y, 0);

                glTexCoord2f(1, 1);
                glVertex3f(c->position.x, c->position.y + 32, 0);
                glTexCoord2f(0, 1);
                glVertex3f(c->position.x + 32, c->position.y + 32, 0);
                glTexCoord2f(0, 0);
                glVertex3f(c->position.x + 32, c->position.y, 0);
                glEnd();

        } else {

                glBegin(GL_TRIANGLES);
                glTexCoord2f(0, 1);
                glVertex3f(c->position.x, c->position.y + 32, 0);
                glTexCoord2f(0, 0);
                glVertex3f(c->position.x, c->position.y, 0);
                glTexCoord2f(1, 0);
                glVertex3f(c->position.x + 32, c->position.y, 0);

                glTexCoord2f(0, 1);
                glVertex3f(c->position.x, c->position.y + 32, 0);
                glTexCoord2f(1, 1);
                glVertex3f(c->position.x + 32, c->position.y + 32, 0);
                glTexCoord2f(1, 0);
                glVertex3f(c->position.x + 32, c->position.y, 0);
                glEnd();
        }

        glDisable(GL_TEXTURE_2D);

        glDisable(GL_BLEND);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
}
