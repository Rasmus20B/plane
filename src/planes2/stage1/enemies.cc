#include "enemies.h"

namespace plane {

  Enm burst_enemy01(std::vector<Vec2> pts) noexcept {
    Enm e = enmCreate(Enm {
      .spatial = {
        .move_points = pts,
        .speed = 0.2f,
        .movement = MOVE_SMOOTH,
        .flag = enmMoveFlag::ACCEL_DECEL,
        .special1 = 0.3f,
        .special2 = 0.3f,
      },
      .danmaku = {},
      .sprite = tm.actSprites[0],
    }, pts[0].x(), pts[0].y(), 15, 2, 0);


    BulletMgr b {
      .mode = BulletFlag::AIMED
    };
    b.setCount(2, 3);
    b.setType(BulletSprite::BLADE_01);
    b.setSpeed(3, 2);
    b.setAngle(0, 15);

    e.danmaku[0] = b;
    e.shots[100] = b;
    e.shots[200] = b;
    e.shots[300] = b;

    b.mode = BulletFlag::RING_AIMED;
    b.setCount(4, 4);
    b.setType(BulletSprite::ORB_03);
    b.setSpeed(5, 5);
    b.setAngle(0, 15);

    e.danmaku[1] = b;
    e.shots[150] = b;
    e.shots[250] = b;
    e.shots[350] = b;
    return e;
  }
}
