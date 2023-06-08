#pragma once

#include <vector> 
#include <cstdint> 

#include <raylib.h> 

#define MULTI_T

#ifdef MULTI_T
#include <thread> 
#include <mutex> 
#endif

#include "../common/vector_calc.h"
#include "../common/spline.h"

namespace plane {

  enum class EnemyAttrs {
    ENEMY_ATTR_NORM
  };
  struct Enemy {
    std::vector<Vec2> points;
    uint32_t size;
    uint32_t health;
    float speed;
    Color col;
    bool looped; 
    float spawntime;
  };

  struct EnemyPool {
    std::vector<std::vector<Vec2>> movement_points;
    std::vector<size_t> current_points;
    std::vector<Color> colours;
    std::vector<Vec2> positions;
    std::vector<uint32_t> sizes;
    std::vector<uint32_t> health;
    std::vector<float> speeds;
    std::vector<bool> looped;
    std::vector<float> spawntime;
#ifdef MULTI_T
    std::mutex m;
#endif
  };

  void enemyPoolInit(EnemyPool& ep, size_t sz);
  void addEnemy(EnemyPool& ep, Enemy& e);

}
