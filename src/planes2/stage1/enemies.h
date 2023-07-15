#pragma once

#include "../timeline.h"
#include "stage1.h"
#include "../enm.h"
#include "../bullets.h"

/* TODO: Create a small language that can be read at 
 * runtime and compiled into runtime stage data 
 * (timeline, enemy definitions, etc) 
 *
 *  Example:
 *
 *  burst_enemy01() {
 *    spawn(100, 100);
 *    move(200, 200);
 *    wait(10);
 *    move(300, 200);
 *    shoot(1);
 *    move(700, 200);
 *    despawn();
 *  }
 *
 *  - Movement points get compiled into line interpolations.
 *  - waits get translated into vector of uint16_ts, or added onto line interpolations like noops?
 *  - 
 *
 * */

namespace plane {
  Enm burst_enemy01(std::vector<Vec2> pts) noexcept;
}
// namespace plane {
//   void burst_ship01(stage1 s) {
//
//     /* define bullets. */
//
//     enShoot(relative_frame, int danmaku_index);
//
//     enMove()
//
//   }
//
//   
//   int load_enemies();
// }
