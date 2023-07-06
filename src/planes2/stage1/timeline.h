#pragma once

#include <cstdint>
#include <array>

#include "enemies.h"

#include "../../common/queue.h"

namespace plane {

  struct stage1 {
    void load_state1Timeline(Queue<std::pair<uint32_t, std::array<uint16_t, 16>>, 256>& timeline) {
     
    }
  };
}
