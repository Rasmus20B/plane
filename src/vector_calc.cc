#include "vector_calc.h"

#include <iostream>

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

  float Vec2Len(Vector2 a) {
    return sqrtf(pow(a.x, 2) + pow(a.y, 2));
  }

  float Vec2Dot(Vector2 a, Vector2 b) {
    return (a.x * b.x) + (a.y * b.y);
  }

  float Vec2Angle(Vector2 a, Vector2 b) {
    return acos(Vec2Dot(a, b) / (Vec2Len(a) * Vec2Len(b)));
  }
  Vector2 Vec2Norm(Vector2 a) {
    int mag = Vec2Len(a);
    a.x /= mag;
    a.y /= mag;
    std::cout << "Len : " << mag << ". a.x = " << a.x << ", a.y = " << a.y << "\n";
    return a;
  }
}
