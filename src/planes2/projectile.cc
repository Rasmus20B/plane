#include "projectile.h"

namespace plane {

  void addProjectile(ProjectilePool& pp, Projectile& p) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(pp.m);
#endif
    pp.positions.push_back(p.position);
    pp.radii.push_back(p.radius);
    pp.colours.push_back(p.colour);
    pp.speeds.push_back(p.speed);
    pp.attrs.push_back(p.attr);
    pp.size++;
  }

  void projectilePoolInit(ProjectilePool& pp, size_t sz) {
#ifdef MULTI_T
    std::scoped_lock<std::mutex> lock(pp.m);
#endif
    pp.positions.reserve(sz);
    pp.old_positions.reserve(sz);
    pp.speeds.reserve(sz);
    pp.colours.reserve(sz);
    pp.radii.reserve(sz);
    pp.attrs.reserve(sz);
    pp.size = 0;
  }
}
