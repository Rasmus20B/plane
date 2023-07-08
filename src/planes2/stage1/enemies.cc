#include "enemies.h"

namespace plane {

  Enm burst_enemy01() {
    Enm e = enmCreate(Enm {
      .spatial = {
        .move_points = {{100, 100}, {200, 200}, {300, 500}, {400, 100}, {600, 100}},
        .speed = 3.f,
        .movement = MOVE_SMOOTH,
      },
      .danmaku = {},
      .sprite = tm.actSprites[0],
    }, 200.f, 200.f, 15, 2, 0);


    BulletMgr b {
      .mode = BulletFlag::AIMED
    };
    b.setCount(2, 3);
    b.setOrigin({300, 500});
    b.setType(BulletSprite::ORB_02);
    b.setSpeed(3, 2);
    b.setAngle(0, 15);

    e.danmaku[0] = b;

    e.shots[40] = b;
    e.shots[100] = b;
    e.shots[150] = b;

    return e;
  }

}
