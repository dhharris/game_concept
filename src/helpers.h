#pragma once

#include "common.h"

// Error value for Vector2
static const Vector2 VECTOR2_NULL = {-1, -1};

int Vector2Eq(Vector2 a, Vector2 b);
int ColorEq(Color a, Color b);
Image get_sprite_from_sheet(Image sheet, Vector2 index);
