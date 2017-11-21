#include <stdlib.h>

#include "item.h"

item *item_new()
{
        item *item = malloc(sizeof(item));
        item->position = vec2_zero();
        item->itemtype = ITEMTYPE_NONE;
        item->info = "NULL";
        return item;
}

int char_to_itemtype(char c)
{
        switch(c) {
                case '$':
                        return ITEMTYPE_COINS;
                default:
                        return ITEMTYPE_NONE;
        }
}

const char *item_get_description(int itemtype)
{
        switch(itemtype) {
                case '$':
                        return "Lovely money!";
                default:
                        return "NULL";
        }
}

void item_delete(item *item)
{
        free(item);
}
