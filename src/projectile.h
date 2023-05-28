
#include <raylib.h>

#include <stdlib.h>

namespace plane {
struct Projectile {
  Vector2 pos;
  float speed;
  float size;
  float angle;
  bool live = true;
  bool home = false;
  size_t target;
};
}
