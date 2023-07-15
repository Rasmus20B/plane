#include "move.h"

namespace plane {
  void eMove(EnemySpatial& e) {
    return;
  }

  void pMove(ProjectileSpace& p) {
    auto vec = p.position.norm();
    switch(p.mt) {
      case MoveType::MOVE_CIRCLE: 
        p.position.vec.x += cos(RAD(p.angle)) * p.velocity.vec.x;
        p.position.vec.y +=  sin(RAD(p.angle)) * p.velocity.vec.y;
        p.angle += p.angle_inc;
        break;
      case MoveType::MOVE_NORM:
        p.position.vec = { p.position.vec.x + p.velocity.vec.x, p.position.vec.y + p.velocity.vec.y} ;
        break;
    }
  }
  void pOnMove(ProjectileSpace& p) {

  }
}
