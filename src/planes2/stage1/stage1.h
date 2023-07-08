#pragma once

#include <vector>

#include "../enemy.h"

#include "../texture.h"

#include "../enm.h"
#include "enemies.h"

#include <raylib.h>

namespace plane {
  struct stage1 {
    std::vector<Enm> e;
    std::vector<Texture2D> bg;
    std::vector<AudioStream> bgm;

    void load_enemies();
  };
}
