#pragma once

#include <cstdint> 
#include <unordered_map> 
#include <vector> 

#include <raylib.h> 

#ifdef MULTI_T
#include <thread> 
#include <mutex> 
#endif

#include "../common/vector_calc.h"
#include "../common/spline.h"

#include "projectile.h"
#include "bullets.h"

namespace plane {


  enum class EnemyAttrs {
    ENEMY_ATTR_NORM
  };

  enum class EnemyState {
    ENEMY_MOVING,
    ENEMY_STOPPED
  };

  struct Enemy {
    std::array<BulletMgr, 16> danmaku;
    std::vector<Vec2> points;
    std::vector<int> stopstarts;
    uint32_t size;
    float health;
    float speed;
    size_t spawntime;
    Texture2D sprite;
    bool looped; 
    EnemyState state;
  };

  struct EnemySpatial {
    Vec2 position;
    std::vector<Vec2> points;
    uint32_t current_t;
    uint32_t size;
    float speed;
  };

  struct EnemyPool {
    std::vector<EnemySpatial> space;
    std::vector<std::array<BulletMgr, 16>> dnmku;
    std::vector<Texture2D> sprite;
    std::vector<std::unordered_map<int, ProjectilePool>> shots;
    std::vector<std::vector<int>> stopstarts;
    std::vector<float> health;
    std::vector<float> spawntimes;
    std::vector<EnemyState> states;
    std::vector<bool> looped;
#ifdef MULTI_T
    std::mutex m;
#endif
  };

  void enmCreate(Enemy& e, uint16_t idx, float x, float y, uint16_t health, uint16_t score, uint16_t item);
  void enemyPoolInit(EnemyPool& ep, size_t sz);
  void addEnemy(EnemyPool& ep, Enemy&& e);
  void addEnemy(EnemyPool& ep, Enemy& e);
  void updateEnemies(EnemyPool& ep);
}
