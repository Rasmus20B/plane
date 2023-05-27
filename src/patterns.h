#pragma once
#include <raylib.h> 

#include "config.h"
#include "spline.h"

#include <array>
namespace plane {
Vector2 ltr_pattern(Vector2 cur, Vector2 &prev);
Vector2 golden_ration(Vector2 cur, Vector2& prev);
}
