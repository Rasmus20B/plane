#pragma once

#include <raylib.h>

#include <cmath>

namespace plane {
  Vector2 Vec2Sub(Vector2 a, Vector2 b);
  Vector2 Vec2Add(Vector2 a,  Vector2 b);
  Vector2 Vec2Div(Vector2 a, float b);
  Vector2 Vec2Mul(Vector2 a,  Vector2 b);
  Vector2 Vec2Mul(Vector2 a, float b);
}
