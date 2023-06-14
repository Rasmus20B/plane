#include "projectile.h"

namespace plane {

  void addProjectile(ProjectilePool& pp, Projectile&& p) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(pp.m);
#endif
    pp.spaces.push_back( ProjectileSpace {
      .position = p.position,
      .velocity = p.velocity,
      .old_position = p.position,
      .radius = p.radius,
      .angle = p.angle,
      .delta = 1
    });
    pp.colours.push_back(p.colour);
    pp.spawntime.push_back(p.spawntime);
    pp.attrs.push_back(p.attr);
    pp.live.push_back(true);
    pp.size++;
  }

  void addProjectile(ProjectilePool& pp, Projectile& p) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(pp.m);
#endif
    pp.spaces.push_back( ProjectileSpace {
      .position = p.position,
      .velocity = p.velocity,
      .old_position = p.position,
      .radius = p.radius,
      .angle = p.angle,
      .delta = 1
    });
    pp.colours.push_back(p.colour);
    pp.spawntime.push_back(p.spawntime);
    pp.attrs.push_back(p.attr);
    pp.live.push_back(true);
    pp.size++;
  }
  void projectilePoolInit(ProjectilePool& pp, size_t sz) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(pp.m);
#endif
    pp.spaces.reserve(sz);
    pp.colours.reserve(sz);
    pp.attrs.reserve(sz);
    pp.live.reserve(sz);
    pp.size = 0;
  }
}
