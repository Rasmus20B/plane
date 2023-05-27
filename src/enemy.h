

#include <raylib.h>

#include <functional>

#include "patterns.h"
#include "spline.h"

namespace plane {
using MPattern = std::function<Vector2(Vector2, Vector2&)>;

struct Lifetime {
  constexpr bool contains(float t) {
    return (start <= t && end >= t);
  }
  float start;
  float end;
};
struct Enemy {
  SplinePt pos;
  float prog = 0;
  Spline mpat;
  float spline_t = 0.01;
  float shoot_t;
  float last_shot;
  float speed = 0.01;
  float size;
  Lifetime lt;
  
  bool dead = false;
};
}
