#include "enm.h"

#include "virt_screen.h"

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

    e_tmp.attrs.health = health;

    e_tmp.spatial.pos = {x, y};

    switch(e.spatial.movement) {
      case MOVE_LINEAR:
        break;
      case MOVE_SMOOTH:
        {
          std::vector<SplinePt> pts;
          if(Vector2{x, y} != e.spatial.move_points[0]) {
            pts.push_back({ {gg.x + x,  gg.y + y}, 0});
          }
          for(auto &i : e.spatial.move_points) {
            pts.push_back({ {gg.x + i.vec.x, gg.y + i.vec.y}, 0});
          }
          Spline s(pts, e.spatial.speed);
          e_tmp.spatial.move_points = s.calc_points(0.01, e.spatial.speed, false);
        }
        break;
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
      case enmMoveFlag::ACCEL_DECEL:
        if((e.move_points.size() * 0.5) < e.cur) {
          e.speed -= e.special1;
          if(e.speed < 0) {
            return;
          }
        } else {
          e.speed += e.special1;
        }
        break;
      case NA:
        e.cur += e.speed;
        break;
      default:
        break;
    }
    if(e.speed >= e.move_points.size() - e.cur) {
      e.speed -= e.special1;
      return;
    }
    e.pos = e.move_points[e.cur];
    if(e.cur < e.move_points.size() - e.speed - 1) {
      e.cur += e.speed;
    }
  }
}
