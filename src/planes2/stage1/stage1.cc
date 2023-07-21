#include "stage1.h"
namespace plane {

  void stage1::load_timeline() {
    timeline.push({{30}, {0}});
    timeline.push({{30}, {1}});
  }

  void stage1::load_enemies() {
    std::vector<Vec2> pts = {{-50, 0}, {0, 0}, {100, 300}, {200, 200}, {300, 300}, {400, 200}, {600, 100}, {650, 100}};
    std::vector<Vec2> pts_rev;
    std::transform(pts.begin(), pts.end(), std::back_inserter(pts_rev), [](const Vec2 v) {
          return Vec2(gg.c - v.vec.x, v.vec.y);
        });

    e.push_back(burst_enemy01(pts));
    e.push_back(burst_enemy01(pts_rev));
  }

}
