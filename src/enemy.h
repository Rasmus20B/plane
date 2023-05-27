

#include <raylib.h>

#include <functional>

#include "patterns.h"
#include "spline.h"

namespace plane {
using MPattern = std::function<Vector2(Vector2, Vector2&)>;
struct Enemy {
  Vector2 pos;
  Vector2 prev_pos;
  Spline mpat;
  float spline_t;
  float shoot_t;
  float last_shot;
  float speed = 0.01;
  bool dead = false;
};
}
