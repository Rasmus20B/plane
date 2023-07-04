#pragma once

#include <cstdint>

#define DRAW_HITBOX
// #define MULTI_T

namespace plane {
struct Config {
  float screen_width;
  float screen_height;
};

static Config config = { 720, 1080 };
}
