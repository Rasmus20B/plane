#include "enemy.h"

namespace plane {

  void enemyPoolInit(EnemyPool& ep, size_t sz) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(ep.m);
#endif
    ep.space.reserve(sz);
    ep.health.reserve(sz);
    ep.colours.reserve(sz);
    ep.looped.reserve(sz);
    ep.spawntime.reserve(sz);
    ep.last_shots.reserve(sz);
    ep.sprite.reserve(sz);
  }
  
  void addEnemy(EnemyPool& ep, Enemy& e) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(ep.m);
#endif

    ep.space.push_back(EnemySpatial {
        .position = e.points.front(),
        .points = e.points,
        .current_t = 0,
        .size = e.size,
        .speed = e.speed,
        });
    ep.colours.push_back(e.col);
    ep.health.push_back(e.health);
    ep.looped.push_back(e.looped);
    ep.spawntime.push_back(e.spawntime);
    ep.last_shots.push_back(0);
    ep.sprite.push_back(e.sprite);
  }

  void addEnemy(EnemyPool& ep, Enemy&& e) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(ep.m);
#endif
    ep.space.push_back( std::move(EnemySpatial {
        .position = e.points.front(),
        .points = e.points,
        .current_t = 0,
        .size = e.size,
        .speed = e.speed,
        }));
    ep.colours.push_back(e.col);
    ep.health.push_back(e.health);
    ep.looped.push_back(e.looped);
    ep.spawntime.push_back(e.spawntime);
    ep.last_shots.push_back(0);
    ep.sprite.push_back(e.sprite);
  }

  void updateEnemies(EnemyPool& ep) {
  }
}
