#include "stage1.h"

namespace plane {

  int stage1::loop() {

    /*
    spawn(enemytype, 100)
    spawn(projectile, 100)


    */


    return 0;
  }
}
/*
 *
 * TASK(burst_enemy, positions) {
 *  while(notdead, t <= lt.second) {
 *    enemyprojectiles.push_back(pos, pattern, etc);
 *    WAIT(25)
 *  }
 * }
 *
 * enemy_spawner() {
 *  for(int i = 0; i < 4; i++) {
 *    TASK(burst_enemy, position + 300)
 *    TASK(burst_enemy, position - 300)
 *    WAIT(20);
 *  }
 * }

   While(1) {
    delayed_spawn(enemy_spawner, time);
    delayed_spawn(enemy2_spawner, time);
    delayed_spawn(enemy3, time);
   }

TODO : just use a routine for each enemy, regular function

*/

