#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "raymath.h"
#include "utils.h"

#include "item.h"
#include "level.h"

// This is only populated if an item needs to be rendered, i.e.
// item_counts[ITEM_TYPE] is non-zero
static Texture2D item_texture_map[NUM_ITEM_TYPES];
static int item_counts[NUM_ITEM_TYPES];

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
        // No-op if null
        if (!s) {
                return;
        }
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

Texture2D get_item_texture(int itemtype)
{
        switch (itemtype) {
                case ITEMTYPE_COINS:
                        return LoadTexture("./sprites/sprite-13-13.png");
                case ITEMTYPE_CHEST:
                        return LoadTexture("./sprites/sprite-13-6.png");
                case ITEMTYPE_URN:
                        return LoadTexture("./sprites/sprite-13-8.png");
                case ITEMTYPE_URN_BROKEN:
                        return LoadTexture("./sprites/sprite-13-9.png");
                case ITEMTYPE_SWORD:
                case ITEMTYPE_LONGSWORD:
                case ITEMTYPE_GREATSWORD:
                        return LoadTexture("./sprites/sprite-17-1.png");
                case ITEMTYPE_SPEAR:
                        return LoadTexture("./sprites/sprite-17-4.png");
                default:
                        return LoadTexture("./sprites/sprite-0-13.png");
        }
}

void item_render(item *item)
{
        int type = item->itemtype;
        if (!item_counts[type]) {
                item_texture_map[type] = get_item_texture(type);
        }
        item_counts[type]++;
        // TODO: Render items in different colors
        DrawTextureV(item_texture_map[type], item->position, BLACK);
}

item_stack **item_map_init()
{
        item_stack **item_map =
            calloc(sizeof(item_stack *), MAX_WIDTH * MAX_HEIGHT);

        for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i)
                item_map[i] = item_stack_new();
        return item_map;
}

void item_map_destroy(item_stack **item_map)
{
        for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i)
                item_stack_destroy(item_map[i]);
        free(item_map);
        // Unload any item textures we used
        for (int i = 0; i < NUM_ITEM_TYPES; ++i) {
                if (item_counts[i]) {
                        UnloadTexture(item_texture_map[i]);
                }
        }
        memset(item_counts, 0, sizeof(item_counts));
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
        TraceLog(LOG_INFO, "[%s] Added item %d at position (%.1f, %.1f)",
                 __FILE__, item->itemtype, position.x, position.y);
}

/* Render everything in the item map */
void item_map_render(item_stack **item_map)
{
        int i;
        for (i = 0; i < MAX_WIDTH * MAX_HEIGHT; ++i) {
                /* Only render whatever is on the top each item stack */
                item *top = item_map[i]->item;
                if (top)
                        item_render(top);
        }
}
