#include <stdlib.h>

#include "item.h"

item *item_empty()
{
        item *item = malloc(sizeof(item));
        item->position = vec2_zero();
        item->itemtype = ITEMTYPE_NONE;
        item->desc = "NULL";
        return item;
}

item *item_new(int itemtype)
{
        item *item = item_empty();
        item->desc = item_get_description(itemtype);
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

int char_is_item(char c)
{
        return char_to_itemtype(c) != ITEMTYPE_NONE;
}

const char *item_get_description(int itemtype)
{
        switch(itemtype) {
                case ITEMTYPE_COINS:
                        return "Lovely money!";
                default:
                        return "NULL";
        }
}

void item_delete(item *item)
{
        free(item);
}
