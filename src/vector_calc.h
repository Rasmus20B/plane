#pragma once

#include <raylib.h>

#include <cmath>

namespace plane {
  Vector2 Vec2Sub(Vector2 a, Vector2 b);
  Vector2 Vec2Add(Vector2 a,  Vector2 b);
  Vector2 Vec2Div(Vector2 a, float b);
  Vector2 Vec2Mul(Vector2 a,  Vector2 b);
  Vector2 Vec2Mul(Vector2 a, float b);
  float Vec2Dot(Vector2 a, Vector2 b);
  float Vec2Angle(Vector2 a, Vector2 b);
  Vector2 Vec2len(Vector2 a);
}
