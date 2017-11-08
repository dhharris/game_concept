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

void item_delete(item *item)
{
        free(item);
}

static texture *item_get_texture(item *item)
{
        texture *t;
        switch (item->itemtype) {
                case ITEMTYPE_COINS:
                        t = asset_get(P("./items/coins.dds"));
                        break;
                case ITEMTYPE_CHEST:
                        t = asset_get(P("./items/chest.dds"));
                        break;
                case ITEMTYPE_URN:
                        t = asset_get(P("./items/urn.dds"));
                        break;
                case ITEMTYPE_SWORD:
                        t = asset_get(P("./items/sword.dds"));
                        break;
                default:
                        t = asset_get(P("./items/none.dds"));
                        break;
        }
        return t;
}
