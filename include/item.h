#ifndef item_h
#define item_h
/**
 * Contains all item logic. Rendering is done by the level.
 */
#include "corange.h"

typedef struct {
        int itemtype;
        const char *desc; // Information about the item
        vec2 position;
} item; /* Struct that defines an item */

/* Item types */
#define ITEMTYPE_NONE 0
#define ITEMTYPE_COINS 1 // lovely money!
#define ITEMTYPE_CHEST 2 // Open to see what's inside
#define ITEMTYPE_URN 3   // When struck with a weapon awards random loot
#define ITEMTYPE_SWORD 4 // a basic weapon

#define NUM_ITEM_TYPES 5 // One more than last item #

/* Function interface to items */
// TODO: Render items on top of tiles, remove items from level tiles
item *item_empty();
item *item_new(int itemtype);
void item_delete(item *item);
int char_to_itemtype(char c);
int char_is_item(char c);
const char *item_get_description(int itemtype);

#endif
