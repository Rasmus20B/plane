
#include <raylib.h>

#include "../common/config.h"
#include <cmath>
#include <vector> 

namespace plane {
struct Player {
  Vector2 pos{config.screen_width/2,(config.screen_height/8) * 6};
  float speed = 8.0f;
  short lives = 3;
  float b_size = 0;
  float b_max_size = 300;
  float last_shot = 0;
  bool b_fire = false;
  bool b_charging = false;
  uint32_t d_time = 0;
  Texture2D sprite = LoadTexture("../assets/Sprite-0001.png");
  Texture2D in_sprite = LoadTexture("../assets/soul.png");
  std::vector<size_t> marked;
};
}
