#pragma once

#include <array>

#include "../enemy.h"

namespace plane {
  void declare_enemies(std::array<Enemy, 256>& e);
}
