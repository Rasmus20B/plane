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
    DECCEL,
    ACCEL_DECEL,
  };

  enum enmMoveType {
    MOVE_LINEAR,
    MOVE_SMOOTH
  };

  enum enmShootType {
    SHOOT_FIXED,
    SHOOT_LOOP
  };

  struct EnmSpace {
    std::vector<Vec2> move_points;
    Vec2 abspos={0, 0};
    Vec2 relpos={0,0};
    float absspeed=0;
    float relspeed=0;
    float absang=0;
    float relang=0;
    float cur; // how long along it's path it is, index for movepoints
    enmMoveType movement;
    uint32_t time;

    enmMoveFlag flag;
    float special1;
    float special2;
  };

  struct EnmShoot {
    BulletMgr danmaku[16];
    std::unordered_map<uint32_t, BulletMgr> shots;
    enmShootType type;
  };

  struct EnmAttrs {
    float health;
  };

  struct Enm {
    EnmSpace spatial;
    EnmShoot shooting;
    EnmAttrs attrs;
    Texture2D sprite;
    Color col;
    void draw();
  };

  Enm enmCreate(Enm e, 
    const float x, const float y,
    const uint16_t health, const uint16_t score, 
    const uint16_t items);

  void enmUpdatePos(EnmSpace& e);
}
