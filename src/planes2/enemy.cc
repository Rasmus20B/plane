#include "enemy.h"

namespace plane {

  void enemyPoolInit(EnemyPool& ep, size_t sz) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(ep.m);
#endif
    ep.positions.reserve(sz);
    ep.health.reserve(sz);
    ep.colours.reserve(sz);
    ep.sizes.reserve(sz);
    ep.looped.reserve(sz);
    ep.spawntime.reserve(sz);
  }

  void addEnemy(EnemyPool& ep, Enemy& e) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(ep.m);
#endif
    ep.movement_points.push_back(e.points);
    ep.positions.push_back(ep.movement_points[ep.movement_points.size() - 1][0]);
    ep.health.push_back(e.health);
    ep.colours.push_back(e.col);
    ep.current_points.push_back(0);
    ep.speeds.push_back(e.speed);
    ep.sizes.push_back(e.size);
    ep.looped.push_back(e.looped);
    ep.spawntime.push_back(e.spawntime);
  }
}
