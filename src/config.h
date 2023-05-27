#pragma once

#include <cstdint>

namespace plane {
struct Config {
  uint64_t screen_width;
  uint64_t screen_height;
};

static Config config = { 640, 480 };
}
