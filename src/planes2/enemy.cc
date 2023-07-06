#include "enemy.h"

namespace plane {

  void enmCreate(EnemyPool& ep, const float x, const float y,
    const uint16_t health, const uint16_t score, 
    const uint16_t items);

  void enemyPoolInit(EnemyPool& ep, size_t sz) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(ep.m);
#endif
    ep.space.reserve(sz);
    ep.health.reserve(sz);
    ep.looped.reserve(sz);
    ep.spawntimes.reserve(sz);
    ep.shots.reserve(sz);
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
    ep.health.push_back(e.health);
    ep.looped.push_back(e.looped);
    ep.spawntimes.push_back(e.spawntime);
    // ep.shots.push_back(e.shots);
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
    ep.health.push_back(e.health);
    ep.looped.push_back(e.looped);
    ep.stopstarts.push_back(e.stopstarts);
    ep.spawntimes.push_back(e.spawntime);
    // ep.shots.push_back(e.shots);
    ep.sprite.push_back(e.sprite);
  }

  void updateEnemies(EnemyPool& ep) {
  }
}
