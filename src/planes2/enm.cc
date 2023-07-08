#include "enm.h"

namespace plane {

  void Enm::draw() {
    Vector2 p = {spatial.pos.x(), spatial.pos.y()};
    Rectangle ps_hitbox = Rectangle{
            this->spatial.pos.x(), 
            this->spatial.pos.y(), 
            static_cast<float>(sprite.width ), 
            static_cast<float>(sprite.height )
    };
    DrawTexturePro(
      this->sprite, 
      { 
        0,
        0,
        static_cast<float>(this->sprite.width),
        static_cast<float>(this->sprite.height)
      },
      ps_hitbox,
      {(float)sprite.width / 2 , (float)sprite.height / 2 },
      0, WHITE);
    return;
  }

  Enm enmCreate(const Enm e, 
    const float x, const float y,
    const uint16_t health, const uint16_t score, 
    const uint16_t items) {
  
    Enm e_tmp = e;

    e_tmp.spatial.pos = {x, y};

    switch(e.spatial.movement) {
      case MOVE_LINEAR:
        break;
      case MOVE_SMOOTH:
        {
          std::vector<SplinePt> pts;
          for(auto &i : e.spatial.move_points) {
            pts.push_back({i.vec, 0});
          }
          Spline s(pts, e.spatial.speed);
          e_tmp.spatial.move_points = s.calc_points(0.01, e.spatial.speed, false);
          break;
        }
      default:
        break;
    }

    e_tmp.spatial.cur = 0;
    return e_tmp;
  }
  
  void enmUpdatePos(EnmSpace& e) {
    switch(e.flag) {
      case enmMoveFlag::ACCEL:
        e.speed += e.special1;
        break;
      default:
        break;
    }
    if(e.speed >= e.move_points.size() - e.cur) {
      e.speed -= e.special1;
      return;
    }
    e.pos = e.move_points[e.cur];
    if(e.cur < e.move_points.size() - e.speed - 1)
      e.cur += e.speed;
  }
}
