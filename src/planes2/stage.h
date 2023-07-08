#pragma once

#include <vector>

#include "../common/queue.h"

#include "enm.h"

namespace plane {

  struct Stage {
    std::vector<Enm> enemies;
    std::vector<Texture2D> backgrounds;
    std::vector<AudioStream> music;

    /* Will hold the spawn frame and the index 
       into the enemies defined in vector */
    Queue<std::pair<uint32_t, uint16_t>, 256> e_spawns;

    void load_enemies();
  };

}
