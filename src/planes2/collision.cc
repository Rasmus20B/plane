#include "collision.h"

namespace plane {

  Vec2 project(const Rectangle& r, std::pair<Vec2, Vec2> line) {
    auto dot = line.second.vec.x * (r.x - line.first.vec.x) +
      line.second.vec.y * (r.y - line.first.vec.y);

    return {
      line.first.vec.x + line.second.vec.x * dot,
      line.first.vec.x + line.second.vec.x * dot,
    };
  }

  std::vector<std::pair<Vec2, Vec2>> getAxis(const Rectangle& r, const float radian) {
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
        {Vec2{r.x, r.y} + rx + ry},
        {Vec2{r.x, r.y} + rx + (ry * (-1))},
        {Vec2{r.x, r.y} + (rx * (-1)) + (ry * (-1))},
        {Vec2{r.x, r.y} + (rx * (-1)) + ry}
      }
    };
  }
  bool CheckCollisionRecsAngle(Rectangle r1, float a1, Rectangle r2, float a2) {
  
    auto const lines = getAxis(r2, a2);
    auto const corners = getCorners(r1, a1);

    bool test = true;

    for(auto &l : lines) {
      Vec2 furthers = {0, 0};
      auto halfsize = Vec2{r2.width, r2.height} / 2;
      for(auto &c : corners) {
        Vec2 projected = project(r2, l);
        Vec2 centre = projected - Vec2(r2.x, r2.y);

        bool sign = (centre.vec.x * l.second.vec.x) + (centre.vec.y  * l.second.vec.y) > 0;
        float distance = centre.magnitude() * (sign ? 1 : -1); 


      }
    }

    return true;
  }
}
