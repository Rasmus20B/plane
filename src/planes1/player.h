
#include <raylib.h>

#include "config.h"
#include <cmath>
#include <vector> 

namespace plane {
struct Player {
  Vector2 pos{config.screen_width/2,config.screen_height/2};
  float speed = 8.0f;
  float size = 20.0f;
  float in_size = 2.5f;
  short lives = 3;
  float b_size = 0;
  float b_max_size = 300;
  bool b_fire = false;
  bool b_charging = false;
  uint32_t d_time = 0;
  std::vector<size_t> marked;
};
}
