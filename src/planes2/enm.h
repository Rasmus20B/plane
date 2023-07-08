#pragma once

#include <array>
#include <unordered_map> 

#include "../common/spline.h"
#include "../common/queue.h"

#include "bullets.h"
#include "enemy.h"

namespace plane {

  enum enmSprite {
    RED_SHIP = 0,
  };

  enum enmMoveFlag {
    NA,
    ACCEL,
  };

  enum enmMoveType {
    MOVE_LINEAR,
    MOVE_SMOOTH
  };

  struct EnmSpace {
    std::vector<Vec2> move_points;
    Vec2 pos;
    float speed;
    float special1;
    float special2;
    enmMoveType movement;
    enmMoveFlag flag;
    uint16_t cur; // how long along it's path it is, index for movepoints
  };

  struct Enm {
    EnmSpace spatial;
    BulletMgr danmaku[16];
    std::unordered_map<uint32_t, BulletMgr> shots;
    Texture2D sprite;

    void draw();
  };

  Enm enmCreate(Enm e, 
    const float x, const float y,
    const uint16_t health, const uint16_t score, 
    const uint16_t items);

  void enmUpdatePos(EnmSpace& e);
}
