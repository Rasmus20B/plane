#pragma once

#include "../common/vector_calc.h"

#include <raylib.h> 
#include <vector>

namespace plane {
  enum class ProjectileAttributes {
    PROJECTILE_ATTRIBUTES_NORM = 0,
    PROJECTILE_ATTRIBUTES_HOMING = 1,
    PROJECTILE_ATTRIBUTES_SIZE = 2
  };
  struct Projectile {
    Vec2 position; 
    Vec2 old_position;
    float speed;
    float radius;
    Color colour;
    ProjectileAttributes attr;
    float spawntime;
    bool live;
  };

  struct ProjectilePool {
    size_t size;
    std::vector<Vec2> positions;
    std::vector<Vec2> old_positions;
    std::vector<float> speeds;
    std::vector<float> radii;
    std::vector<float> spawntime;
    std::vector<Color> colours;
    std::vector<ProjectileAttributes> attrs;
    std::vector<bool> live;
#ifdef MULTI_T
    std::mutex m;
#endif
  };

  void projectilePoolInit(ProjectilePool& pp, size_t sz);
  void addProjectile(ProjectilePool& pp, Projectile&& p);
  void addProjectile(ProjectilePool& pp, Projectile& p);
}
