#include <stdlib.h>
#include <assert.h>

#include "raymath.h"

#include "item.h"
#include "level.h"

item *item_empty()
{
        item *item = malloc(sizeof(item));
        item->position = Vector2Zero();
        item->itemtype = ITEMTYPE_NONE;
        return item;
}

item *item_new(int itemtype, Vector2 position)
{
        item *item = item_empty();
        item->itemtype = itemtype;
        item->count = 1;
        item->position = position;
        return item;
}

void item_destroy(item *item)
{
        free(item);
}

item_stack *item_stack_new()
{
        item_stack *head = malloc(sizeof(item_stack));
        head->item = NULL;
        head->next = NULL;
        return head;
}

void item_stack_destroy(item_stack *s)
{
        while (s->next) {
                item_stack *tmp = s->next;
                item_destroy(s->item);
                free(s);
                s = tmp;
        }
}

int item_stack_count(item_stack *head)
{
        int count = 0;
        while (head) {
                head = head->next;
                count++;
        }

        return count;
}

void item_stack_push(item_stack *head, item *item)
{
        if (!head->item) {
                head->item = item;
                return;
        }
        item_stack *cur = head;
        while (cur->next) cur = cur->next;

        cur->next = malloc(sizeof(item_stack));
        cur->next->item = item;
        cur->next->next = NULL;
}

item *item_stack_pop(item_stack **head)
{
        if (!(*head))
                return NULL;
        item_stack *next = (*head)->next;
        item *ret = (*head)->item;
        free(*head);
        *head = next;

        return ret;
}

int char_to_itemtype(char c)
{
        switch (c) {
                case '$':
                        return ITEMTYPE_COINS;
                default:
                        return ITEMTYPE_NONE;
        }
}

int char_is_item(char c)
{
        return char_to_itemtype(c) != ITEMTYPE_NONE;
}

const char *item_get_description(int itemtype)
{
        switch (itemtype) {
                case ITEMTYPE_COINS:
                        return "Lovely money!";
                default:
                        return "NULL";
        }
}

Texture2D *item_get_texture(int itemtype)
{
        // texture *t;
        // switch (itemtype) {
        //         case ITEMTYPE_COINS:
        //                 t = asset_get(P("./sprites/sprite-13-13.dds"));
        //                 break;
        //         case ITEMTYPE_CHEST:
        //                 t = asset_get(P("./sprites/sprite-13-6.dds"));
        //                 break;
        //         case ITEMTYPE_URN:
        //                 t = asset_get(P("./sprites/sprite-13-8.dds"));
        //                 break;
        //         case ITEMTYPE_URN_BROKEN:
        //                 t = asset_get(P("./sprites/sprite-13-9.dds"));
        //                 break;
        //         case ITEMTYPE_SWORD:
        //         case ITEMTYPE_LONGSWORD:
        //         case ITEMTYPE_GREATSWORD:
        //                 t = asset_get(P("./sprites/sprite-17-1.dds"));
        //                 break;
        //         case ITEMTYPE_SPEAR:
        //                 t = asset_get(P("./sprites/sprite-17-4.dds"));
        //                 break;
        //         default:
        //                 t = asset_get(P("./sprites/sprite-0-13.dds"));
        //                 break;
        // }
        // return t;
        return 0;
}

void item_render_one(item *item, Vector2 camera_position)
{
        // glMatrixMode(GL_PROJECTION);
        // glPushMatrix();
        // glLoadIdentity();
        // glOrtho(camera_position.x - graphics_viewport_width() / 2,
        //         camera_position.x + graphics_viewport_width() / 2,
        //         -camera_position.y + graphics_viewport_height() / 2,
        //         -camera_position.y - graphics_viewport_height() / 2, -1, 1);

        // glMatrixMode(GL_MODELVIEW);
        // glPushMatrix();
        // glLoadIdentity();

        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // glEnable(GL_TEXTURE_2D);

        // texture *item_tex = item_get_texture(item->itemtype);
        // glBindTexture(GL_TEXTURE_2D, texture_handle(item_tex));

        // glBegin(GL_TRIANGLES);
        // glTexCoord2f(0, 1);
        // glVertex3f(item->position.x, item->position.y + TILE_SIZE, 0);
        // glTexCoord2f(0, 0);
        // glVertex3f(item->position.x, item->position.y, 0);
        // glTexCoord2f(1, 0);
        // glVertex3f(item->position.x + TILE_SIZE, item->position.y, 0);

        // glTexCoord2f(0, 1);
        // glVertex3f(item->position.x, item->position.y + TILE_SIZE, 0);
        // glTexCoord2f(1, 1);
        // glVertex3f(item->position.x + TILE_SIZE, item->position.y + TILE_SIZE,
        //            0);
        // glTexCoord2f(1, 0);
        // glVertex3f(item->position.x + TILE_SIZE, item->position.y, 0);
        // glEnd();

        // glDisable(GL_TEXTURE_2D);
        // glDisable(GL_BLEND);

        // glMatrixMode(GL_PROJECTION);
        // glPopMatrix();

        // glMatrixMode(GL_MODELVIEW);
        // glPopMatrix();
}

item_stack **item_map_init()
{
        item_stack **item_map = calloc(sizeof(item_stack *), MAX_WIDTH * MAX_HEIGHT);
        int i;

        for (i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i)
                item_map[i] = item_stack_new();
        return item_map;
}

void item_map_destroy(item_stack **item_map)
{
        int i;
        for (i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i)
                item_stack_destroy(item_map[i]);
        free(item_map);
        item_map = NULL;
}

/* Update item logic */
void item_map_update(item_stack **item_map)
{
        /* Perform any item map updates here, such as pots being broken, etc */
}

item_stack *item_map_stack_at(item_stack **item_map, Vector2 position)
{
        int x = floor(position.x / TILE_SIZE);
        int y = floor(position.y / TILE_SIZE);

        assert(x >= 0);
        assert(y >= 0);
        assert(x < MAX_WIDTH);
        assert(y < MAX_HEIGHT);

        return item_map[x + y * MAX_WIDTH];
}

void item_map_add_item(item_stack **item_map, int itemtype, Vector2 position)
{
        item *item = item_new(itemtype, position);
        item_stack *s = item_map_stack_at(item_map, position);
        item_stack_push(s, item);
}

/* Render everything in the item map */
void item_map_render(item_stack **item_map, Vector2 camera_position)
{
        int i;
        for (i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i) {
                /* Only render whatever is on the top each item stack */
                item *top = item_map[i]->item;
                if (top)
                        item_render_one(top, camera_position);
        }
}
