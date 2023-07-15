#pragma once

#include <vector>
#include <array>
#include "../common/vector_calc.h"

#ifdef __aarch64__
  #include <arm_neon.h>
#elif __x86_64__
  #include <immintrin.h>
  #include <pmmintrin.h>
#endif

#include <raylib.h>  

namespace plane {
  using line = std::pair<Vec2, Vec2>;
  std::vector<line> getAxis(const Rectangle& r, const float radian);
  std::array<Vec2, 4> getCorners(const Rectangle& r, const float radian);
  std::array<Vec2, 4> getCornersSIMD(const Rectangle& r, const float radian);
  bool checkContains(std::array<Vec2, 4> cs1, std::array<Vec2, 4> cs2);
  bool checkContainsSIMD(std::array<Vec2, 4> cs1, std::array<Vec2, 4> cs2);
  bool CheckCollisionRecsAngle(const Rectangle& r1, const float a1, const Rectangle& r2, const float a2);
  Vec2 project(line);
}
