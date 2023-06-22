#pragma once

#include <vector>
#include <array>
#include "../common/vector_calc.h"

#include <raylib.h>  

namespace plane {

  using line = std::pair<Vec2, Vec2>;
  std::vector<line> getAxis(const Rectangle& r, const float radian);
  std::array<Vec2, 4> getCorners(const Rectangle& r, const float radian);
  bool CheckCollisionRecsAngle(const Rectangle& r1, const float a1, const Rectangle& r2, const float a2);
  Vec2 project(line);
}
