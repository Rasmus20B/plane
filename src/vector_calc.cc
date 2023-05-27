#include "vector_calc.h"

namespace plane {
  Vector2 Vec2Sub(Vector2 a, Vector2 b) {
    return {a.x - b.x, a.y - b.y};
  }

  Vector2 Vec2Add(Vector2 a, Vector2 b) {
    return {a.x + b.x, a.y + b.y};
  }

  Vector2 Vec2Div(Vector2 a, float b) {
    return {fabsf(a.x) / b, fabsf(a.y) / b};
  }

  Vector2 Vec2Mul(Vector2 a, float b) {
    return {a.x * b, a.y * b};
  }
  
  Vector2 Vec2Mul(Vector2 a,  Vector2 b) {
    return {};
  }
}
