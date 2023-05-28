
#include <raylib.h>

#include "config.h"
#include <cmath>

namespace plane {
struct Player {
  Vector2 pos{fdiv(config.screen_width,2),fdiv(config.screen_height,2)};
  float speed = 8.0f;
  float size = 20.0f;
  float in_size = 2.5f;
  short lives = 3;
};
}
