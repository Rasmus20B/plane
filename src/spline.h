#pragma once
#include <array> 
#include <cmath>
#include <raylib.h>

#include "vector_calc.h"

namespace plane {
struct Spline {
  Vector2 getPoint(Vector2);
  Vector2 Interpolate(float t, float alpha = 0.5f);

  std::array<Vector2, 4> points;
};
}
