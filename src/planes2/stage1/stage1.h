#pragma once

#include <vector>
#include <algorithm>

#include "../enemy.h"

#include "../texture.h"

#include "../enm.h"
#include "enemies.h"
#include "../virt_screen.h"

#include <raylib.h>

namespace plane {
  struct stage1 {
    std::vector<Enm> e;
    std::vector<Texture2D> bg;
    std::vector<AudioStream> bgm;
    Queue<std::pair<uint32_t, std::vector<uint16_t>>, 256> timeline;

    void load_enemies();
    void load_timeline();
  };
}
