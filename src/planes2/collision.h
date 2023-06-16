#pragma once

#include <vector>
#include <array>
#include "../common/vector_calc.h"

#include <raylib.h>  

namespace plane {
  std::vector<std::pair<Vec2, Vec2>> getAxis(const Rectangle& r, const float radian);
  std::array<Vec2, 4> getCorners(const Rectangle& r, const float radian);
  bool CheckCollisionRecsAngle(Rectangle r1, float a1, Rectangle r2, float a2);
  Vec2 project(std::pair<Vec2, Vec2>);
}
