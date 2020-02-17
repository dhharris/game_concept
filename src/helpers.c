#include "helpers.h"

int ColorEq(Color a, Color b)
{
        if (a.r != b.r)
                return 0;
        if (a.b != b.b)
                return 0;
        if (a.g != b.g)
                return 0;
        return 1;
}

Image get_sprite_from_sheet(Image sheet, Vector2 index)
{
        return ImageFromImage(sheet, (Rectangle){
                                         index.x * TILE_SIZE,
                                         index.y * TILE_SIZE,
                                         (index.x + 1) * TILE_SIZE,
                                         (index.y + 1) * TILE_SIZE,
                                     });
}
