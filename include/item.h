#ifndef item_h
#define item_h
/**
 * Contains all item logic. Rendering is done by the level.
 */
#include "corange.h"

/* Important constants */
#define INVENTORY_SIZE 10

typedef struct {
        int itemtype;
        char *info; // Information about the item
        vec2 position;
} item; /* Struct that defines an item */

/* Item types */
#define ITEMTYPE_NONE 0
#define ITEMTYPE_COINS 1 // lovely money!
#define ITEMTYPE_CHEST 2 // Open to see what's inside
#define ITEMTYPE_URN 3   // When struck with a weapon awards random loot
#define ITEMTYPE_SWORD 4 // a basic weapon

#define NUM_ITEM_TYPES 5 // One more than last item #

#endif
