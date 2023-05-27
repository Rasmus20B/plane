#pragma once

#include <cstdint>

namespace plane {
struct Config {
  double screen_width;
  double screen_height;
};

static Config config = { 640, 480 };
}
