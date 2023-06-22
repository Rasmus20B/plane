#include "collision.h"
#include <algorithm>

namespace plane {

  Vec2 project(const Rectangle& r, const line line) {
    auto dot = line.second.vec.x * (r.x - line.first.vec.x) +
      line.second.vec.y * (r.y - line.first.vec.y);

    return {
      line.first.vec.x + line.second.vec.x * dot,
      line.first.vec.x + line.second.vec.x * dot,
    };
  }

  std::vector<line> getAxis(const Rectangle& r, const float radian) {
    const Vec2 ox = { 1, 0 };
    const Vec2 oy = { 0, 1 };

    const Vec2 rx = ox.rotate(radian);
    const Vec2 ry = oy.rotate(radian);

    return { {
      { {r.x, r.y}, {rx.vec.x, rx.vec.y } },
      { {r.x, r.y}, {ry.vec.x, ry.vec.y } },
    }
    };

  }

  std::array<Vec2, 4> getCorners(const Rectangle& r, const float radian) {
    auto axis = getAxis(r, radian);
    auto rx = axis[0].second * (r.width/2.0f);
    auto ry = axis[1].second * (r.height/2.0f);
    return {
      {
        {Vec2{r.x, r.y} + rx + ry + Vec2{r.width / 2, r.height / 2}},
        {Vec2{r.x, r.y} + rx + (ry * (-1)) + Vec2{r.width / 2, r.height / 2}},
        {Vec2{r.x, r.y} + (rx * (-1)) + (ry * (-1)) + Vec2{r.width / 2, r.height / 2}},
        {Vec2{r.x, r.y} + (rx * (-1)) + ry + Vec2{r.width / 2, r.height / 2}}
      }
    };
  }

  bool CheckCollisionRecsAngle(const Rectangle& r1, const float a1, const Rectangle& r2, const float a2) {

    auto cs1 = getCorners(r1, a1);
    auto cs2 = getCorners(r2, a2);


    float minx1 = INFINITY;
    float maxx1 = -INFINITY;
    float miny1 = INFINITY;
    float maxy1 = -INFINITY;
    for(int i = 0; i < cs1.size(); ++i) {
      if(minx1 > cs1[i].vec.x) minx1 = cs1[i].vec.x;
      if(maxx1 < cs1[i].vec.x) maxx1 = cs1[i].vec.x;
      if(miny1 > cs1[i].vec.x) miny1 = cs1[i].vec.y;
      if(maxy1 < cs1[i].vec.x) maxy1 = cs1[i].vec.y;
    }

    float minx2 = INFINITY;
    float maxx2 = -INFINITY;
    float miny2 = INFINITY;
    float maxy2 = -INFINITY;
    for(int i = 0; i < cs2.size(); ++i) {
      if(minx2 > cs2[i].vec.x) minx2 = cs2[i].vec.x;
      if(maxx2 < cs2[i].vec.x) maxx2 = cs2[i].vec.x;
      if(miny2 > cs2[i].vec.x) miny2 = cs2[i].vec.y;
      if(maxy2 < cs2[i].vec.x) maxy2 = cs2[i].vec.y;
    }

    bool hit = false;
    if(!(minx1 >= minx2 && minx1 <= maxx2 || maxx1 >= minx2 && maxx1 <= maxx2)) {
      return false;
    }
    if(!(miny1 >= miny2 && miny1 <= maxy2 || maxy1 >= miny2 && maxy1 <= miny2)) {
      return false;
    }
    if(!(minx2 >= minx1 && minx2 <= maxx1 || maxx2 >= minx1 && maxx2 <= maxx1)) {
      return false;
    }
    // if(!(miny2 >= miny1 && miny2 <= maxy1 || maxy2 >= miny1 && maxy2 <= miny1)) {
    //   return false;
    // }


    return true;
  }
  // bool CheckCollisionRecsAngle(const Rectangle& r1, const float a1, const Rectangle& r2, const float a2) {
  // 
  //   auto const lines = getAxis(r2, a2);
  //   auto const corners = getCorners(r1, a1);
  //
  //   bool test = true;
  //
  //   for(auto &l : lines) {
  //     Vec2 furthers = {0, 0};
  //     auto halfsize = Vec2{r2.width, r2.height} / 2;
  //     for(auto &c : corners) {
  //       Vec2 projected = project(r2, l);
  //       Vec2 centre = projected - Vec2(r2.x, r2.y);
  //
  //       bool sign = (centre.vec.x * l.second.vec.x) + (centre.vec.y  * l.second.vec.y) > 0;
  //       float distance = centre.magnitude() * (sign ? 1 : -1); 
  //
  //
  //     }
  //   }
  //
  //   return true;
  // }
}
