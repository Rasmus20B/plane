#pragma once

#include "../common/vector_calc.h"

#include <raylib.h> 
#include <vector>
#include <functional>

namespace plane {
  enum class ProjectileAttributes {
    PROJECTILE_ATTRIBUTES_NORM = 0,
    PROJECTILE_ATTRIBUTES_AIMED = 1,
    PROJECTILE_ATTRIBUTES_RING_AIMED = 2,
    PROJECTILE_ATTRIBUTES_RING_AROUND = 4,
    PROJECTILE_ATTRIBUTES_SIZE 
  };

  /* Used for determining collision detection system */
  enum class ProjectileShape {
    PS_RECT = 0,
    PS_CIRC = 1,
  };

  enum class MoveType {
    MOVE_NORM = 0,
    MOVE_CIRCLE = 1,
  };
  struct Projectile {
    Vec2 position; 
    Vec2 old_position;
    Vec2 velocity;
    float speed;
    float angle;
    float angle_inc;
    ProjectileAttributes attr;
    Texture2D sprite;
    ProjectileShape shape;
    float spawntime;
    float deadtime;
    MoveType mt;
    bool live;
  };

  struct ProjectileSpace {
    Vec2 position;
    Vec2 velocity;
    Vec2 old_position;
    ProjectileShape shape;
    float angle;
    float angle_inc;
    float speed;
    MoveType mt;
  };

  struct ProjectilePool {
    std::vector<ProjectileSpace> spaces;
    std::vector<float> spawntime;
    std::vector<float> deadtime;
    std::vector<Texture2D> sprite;
    std::vector<ProjectileAttributes> attrs;
    std::vector<bool> live;
#ifdef MULTI_T
    std::mutex m;
#endif
    size_t size;
  };

  void projectilePoolInit(ProjectilePool& pp, size_t sz);
  void addProjectile(ProjectilePool& pp, Projectile&& p);
  void addProjectile(ProjectilePool& pp, Projectile& p);
}
