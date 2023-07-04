#include "collision.h"

#include <iostream> 
#include <chrono>
#include <algorithm>
#include <limits>
namespace plane {
  std::vector<line> getAxis(const Rectangle& r, const float radian) {
    const Vec2 ox = { 1, 0 };
    const Vec2 oy = { 0, 1 };
    

    const Vec2 rx = ox.rotate(radian);
    const Vec2 ry = oy.rotate(radian);

    return {{
      { {r.x, r.y}, {rx.vec.x, rx.vec.y} },
      { {r.x, r.y}, {ry.vec.x, ry.vec.y} },
      }};
  }

  std::array<Vec2, 4> getCorners(const Rectangle& r, const float radian) {
    auto cx = 0.5f * r.width;
    auto cy = 0.5f * r.height;

    float ca = cos(radian);
    float sa = sin(radian);

    float r1x = -cx * ca - cy * sa;
    float r1y = -cx * sa + cy * ca;
    float r2x = cx * ca - cy * sa;
    float r2y = cx * sa + cy * ca;

    return {
      {
        {r.x + r1x, r.y + r1y},
        {r.x + r2x, r.y + r2y},
        {r.x - r1x, r.y - r1y},
        {r.x - r2x, r.y - r2y},
      }
    };
  }

  bool CheckCollisionRecsAngle(const Rectangle& r1, const float a1, const Rectangle& r2, const float a2) {

    auto cs1 = getCorners(r1, a1);
    auto cs2 = getCorners(r2, a2);

#ifdef __arm64__

#elif __x86_64__

#endif
    float min1 = std::numeric_limits<float>().max();
    float max1 = std::numeric_limits<float>().min();
    float min2 = std::numeric_limits<float>().max();
    float max2 = std::numeric_limits<float>().min();

    for(int i = 0; i < 4; ++i) {
      float dx = cs1[(i+1) % 4].vec.x - cs1[i].vec.x;
      float dy = cs1[(i+1) % 4].vec.y - cs1[i].vec.y;
      Vec2 edge = cs1[(i+1) % 4] - cs1[i];
      Vector2 axis = {-edge.vec.y, edge.vec.x};

      for(int j = 0; j < 4; ++j) {
        float proj1 = cs1[j].dot(Vec2{axis});
        float proj2 = cs2[j].dot(Vec2{axis});

        if(proj1 < min1) min1 = proj1;
        if(proj1 > max1) max1 = proj1;
        if(proj2 < min2) min2 = proj2;
        if(proj2 > max2) max2 = proj2;
      }
      if(min1 >= max2 || min2 >= max1) {
        return false;
      }
    }

    for(int i = 0; i < 4; ++i) {
      float dx = cs2[(i+1) % 4].vec.x - cs2[i].vec.x;
      float dy = cs2[(i+1) % 4].vec.y - cs2[i].vec.y;
      Vec2 edge = cs2[(i+1) % 4] - cs2[i];
      Vector2 axis = {-edge.vec.y, edge.vec.x};

      for(int j = 0; j < 4; ++j) {
        float proj1 = cs2[j].dot(Vec2{axis});
        float proj2 = cs1[j].dot(Vec2{axis});

        if(proj1 < min1) min1 = proj1;
        if(proj1 > max1) max1 = proj1;
        if(proj2 < min2) min2 = proj2;
        if(proj2 > max2) max2 = proj2;
      }
      if(min1 >= max2 || min2 >= max1) {
        return false;
      }
    }
    return true;
  }
}
