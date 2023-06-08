#pragma once

#include <vector> 
#include <cstdint> 

#include <raylib.h> 

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
    std::vector<SplinePt> points;
    uint32_t size;
    uint32_t health;
    float speed;
    Color col;
  };

  struct EnemyPool {
    std::vector<size_t> current_points;
    std::vector<SplinePt> positions;
    std::vector<uint32_t> sizes;
    std::vector<Color> colours;
    std::vector<uint32_t> health;
    std::vector<float> speeds;
    std::vector<std::vector<SplinePt>> movement_points;
#ifdef MULTI_T
    std::mutex m;
#endif
  };

  void enemyPoolInit(EnemyPool& ep, size_t sz);
  void addEnemy(EnemyPool& ep, Enemy& e);

}
