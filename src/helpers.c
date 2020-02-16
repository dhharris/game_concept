#include "helpers.h"

Image get_sprite_from_sheet(Image sheet, Vector2 index)
{
        return ImageFromImage(sheet, (Rectangle){
                index.x * TILE_SIZE,
                index.y * TILE_SIZE,
                (index.x + 1) * TILE_SIZE,
                (index.y + 1) * TILE_SIZE,
        });
}
