#pragma once

#include "common.h"
/**
 * Contains all item logic. Rendering is done by the level.
 */

typedef struct {
        int itemtype;
        int count;
        Vector2 position;
} item; /* Struct that defines an item */

typedef struct item_node {
        item *item;
        struct item_node *next;
} item_stack; /* Defines a stack of items as a linked list */

/* Item types */
#define ITEMTYPE_NONE 0
#define ITEMTYPE_COINS 1      // Lovely money!
#define ITEMTYPE_CHEST 2      // I wonder what's inside?
#define ITEMTYPE_URN 3        // A ceramic vessel
#define ITEMTYPE_URN_BROKEN 4 // This was used to hold things
#define ITEMTYPE_SWORD 5      // A basic melee weapon
#define ITEMTYPE_LONGSWORD 6  // a sword with a longer grip
#define ITEMTYPE_GREATSWORD 7 // a large sword intended for two-handed use
#define ITEMTYPE_SPEAR 8      // long and pointy

#define NUM_ITEM_TYPES 9 // One more than last item #

/* Function interface to items */
item *item_empty();
item *item_new(int itemtype, Vector2 position);
void item_destroy(item *item);

/* Function interface to item_stack */
item_stack *item_stack_new();
void item_stack_destroy(item_stack *s);
void item_stack_push(item_stack *head, item *item);
item *item_stack_pop(item_stack **head);
int item_stack_count(item_stack *head);

/* Rendering and ground items logic */
item_stack **item_map_init();
void item_map_destroy(item_stack **item_map);
void item_map_update(item_stack **item_map);
void item_map_render(item_stack **item_map);
item_stack *item_map_stack_at(item_stack **item_map, Vector2 position);
void item_map_add_item(item_stack **item_map, int itemtype, Vector2 position);

/* Misc helper functions */
int char_to_itemtype(char c);
int char_is_item(char c);
const char *item_get_description(int itemtype);
