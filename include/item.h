#ifndef item_h
#define item_h

#include "corange.h"

/* Important constants */
#define ITEM_LIMIT 255 // Maximum amount of items displayable at once

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

#endif
