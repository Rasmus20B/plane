#include "collision.h"
#include <xmmintrin.h>

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
    const float cx = 0.5f * r.width;
    const float cy = 0.5f * r.height;

    const float ca = cos(radian);
    const float sa = sin(radian);

    const float r1x = -cx * ca - cy * sa;
    const float r1y = -cx * sa + cy * ca;
    const float r2x = cx * ca - cy * sa;
    const float r2y = cx * sa + cy * ca;

    return {
      {
        {r.x + r1x, r.y + r1y},
        {r.x + r2x, r.y + r2y},
        {r.x - r1x, r.y - r1y},
        {r.x - r2x, r.y - r2y},
      }
    };
  }

  std::array<Vec2, 4> getCornersSIMD(const Rectangle& r, const float radian) {
    float cx = 0.5f * r.width;
    float cy = 0.5f * r.height;

    float ca = cos(radian);
    float sa = sin(radian);
#ifdef __arm64__
#elif __x86_64__
    __m128 cxs = { -cx, -cx, cx, cx };
    __m128 css = { ca, sa, ca, sa };
    __m128 cys = { -cy, cy, -cy, cy };
    __m128 csr = { sa, ca, sa, ca };

    __m128 r1 = _mm_mul_ps(cxs,  css);
    __m128 r2 = _mm_mul_ps(cys, csr);
    __m128 res = _mm_add_ps(r1, r2);

    return {
      {
        {r.x + res[0], r.y + res[1]},
        {r.x + res[2], r.y + res[3]},
        {r.x - res[0], r.y - res[1]},
        {r.x - res[2], r.y - res[3]}
      }
    };
#else
#endif
  }
  bool CheckCollisionRecsAngle(const Rectangle& r1, const float a1, const Rectangle& r2, const float a2) {

    auto cs1 = getCorners(r1, a1);
    auto cs2 = getCorners(r2, a2);


    for(int i = 0; i < 4; ++i) {
      float dx = cs1[(i+1) % 4].vec.x - cs1[i].vec.x;
      float dy = cs1[(i+1) % 4].vec.y - cs1[i].vec.y;
      Vec2 edge = cs1[(i+1) % 4] - cs1[i];
      Vector2 axis = {-edge.vec.y, edge.vec.x};

      float min1 = std::numeric_limits<float>().max();
      float max1 = std::numeric_limits<float>().min();
      float min2 = std::numeric_limits<float>().max();
      float max2 = std::numeric_limits<float>().min();
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

      float min1 = std::numeric_limits<float>().max();
      float max1 = std::numeric_limits<float>().min();
      float min2 = std::numeric_limits<float>().max();
      float max2 = std::numeric_limits<float>().min();
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
