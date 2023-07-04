#pragma once
#include <vector>
#include <unordered_map>

#include "enemy.h"

namespace plane {

  using timeline = std::unordered_map<uint32_t, Enemy&>;

}
