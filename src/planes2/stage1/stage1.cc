#include "stage1.h"
namespace plane {

  void stage1::load_enemies() {
    std::vector<Vec2> pts = {{-50, 0}, {0, 0}, {100, 300}, {200, 200}, {300, 300}, {400, 200}, {600, 100}, {700, 100}};
    std::vector<Vec2> pts_rev;
    std::transform(pts.begin(), pts.end(), std::back_inserter(pts_rev), [](const Vec2 v) {
          return Vec2(config.screen_width - v.vec.x, v.vec.y);
        });

    e.push_back(burst_enemy01(pts));
    e.push_back(burst_enemy01(pts_rev));
  }

}
