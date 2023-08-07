#include "enemies.h"

namespace plane {

  Enm burst_enemy01(std::vector<Vec2> pts) noexcept {
    Enm e = enmCreate(Enm {
      .spatial = {
        .move_points = pts,
        .absspeed = 0.2,
        .movement = MOVE_SMOOTH,
        .flag = enmMoveFlag::ACCEL_DECEL,
        .special1 = 0.3f,
        .special2 = 0.3f,
      },
      .sprite = tm.actSprites[0],
    }, pts[0].x(), pts[0].y(), 15, 2, 0);


    BulletMgr b {
      .mode = BulletFlag::AIMED
    };
    b.setCount(2, 3);
    b.setType(BulletSprite::BLADE_01);
    b.setSpeed(3, 2);
    b.setAngle(0, 15);

    e.shooting.danmaku[0] = b;
    e.shooting.shots[100] = b;
    e.shooting.shots[200] = b;
    e.shooting.shots[300] = b;

    b.mode = BulletFlag::RING_AIMED;
    b.setCount(4, 4);
    b.setType(BulletSprite::ORB_03);
    b.setSpeed(5, 5);
    b.setAngle(0, 15);

    e.shooting.danmaku[1] = b;
    e.shooting.shots[150] = b;
    e.shooting.shots[250] = b;
    e.shooting.shots[350] = b;
    return e;
  }
}
