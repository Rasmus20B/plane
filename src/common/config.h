#pragma once

#include <cstdint>

#define DRAW_HITBOX

namespace plane {
struct Config {
  float screen_width;
  float screen_height;
};

static Config config = { 720, 1080 };
}
