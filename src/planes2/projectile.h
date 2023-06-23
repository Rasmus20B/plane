#pragma once

#include "../common/vector_calc.h"

#include <raylib.h> 
#include <vector>
#include <functional>

namespace plane {
  enum class ProjectileAttributes {
    PROJECTILE_ATTRIBUTES_NORM = 0,
    PROJECTILE_ATTRIBUTES_HOMING = 1,
    PROJECTILE_ATTRIBUTES_AIMED= 2,
    PROJECTILE_ATTRIBUTES_SIZE 
  };

  enum class MoveType {
    MOVE_NORM = 0,
    MOVE_CIRCLE = 1,
  };
  struct Projectile {
    Vec2 position; 
    Vec2 old_position;
    Vec2 velocity;
    float angle;
    float angle_inc;
    ProjectileAttributes attr;
    Texture2D sprite;
    float spawntime;
    MoveType mt;
    bool live;
  };

  struct ProjectileSpace {
    Vec2 position;
    Vec2 velocity;
    Vec2 old_position;
    float radius;
    float angle;
    float angle_inc;
    float delta;
    MoveType mt;
  };

  struct ProjectilePool {
    size_t size;
    std::vector<ProjectileSpace> spaces;
    std::vector<float> spawntime;
    std::vector<Texture2D> sprite;
    std::vector<ProjectileAttributes> attrs;
    std::vector<bool> live;
    std::vector<std::function<void()>> onUpdate;
#ifdef MULTI_T
    std::mutex m;
#endif
  };

  void projectilePoolInit(ProjectilePool& pp, size_t sz);
  void addProjectile(ProjectilePool& pp, Projectile&& p);
  void addProjectile(ProjectilePool& pp, Projectile& p);
}
