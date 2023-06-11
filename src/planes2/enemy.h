#pragma once

#include <cstdint> 
#include <unordered_map> 
#include <vector> 

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
    Color col;
    uint32_t size;
    float health;
    float speed;
    float spawntime;
    float last_shot = 0;
    bool looped; 
  };

  struct EnemyPool {
    std::vector<std::vector<Vec2>> movement_points;
    std::vector<size_t> current_points;
    std::vector<Color> colours;
    std::vector<Vec2> positions;
    std::vector<uint32_t> sizes;
    std::vector<float> health;
    std::vector<float> speeds;
    std::vector<bool> looped;
    std::vector<float> spawntime;
    std::vector<float> last_shots;
#ifdef MULTI_T
    std::mutex m;
#endif
  };

  void enemyPoolInit(EnemyPool& ep, size_t sz);
  void addEnemy(EnemyPool& ep, Enemy&& e);
  void addEnemy(EnemyPool& ep, Enemy& e);

}
