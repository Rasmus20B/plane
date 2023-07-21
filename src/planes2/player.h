#pragma once
#include <raylib.h>

#include "../common/config.h"
#include <cmath>
#include <vector> 
#include "virt_screen.h"

namespace plane {

  struct pSpace {
    Vector2 pos{gg.x + (gg.c/ 2), gg.y + (gg.r / 6) * 4};
    float speed = 8.0f;
    bool micro = false;
  };
  struct Player {
    pSpace spatial;
    short lives = 3;
    float b_size = 0;
    float b_max_size = 300;
    float last_shot = 0;
    bool b_fire = false;
    bool b_charging = false;
    uint32_t d_time = 0;
    Texture2D sprite = LoadTexture("../assets/Sprite-0001.png");
    Texture2D in_sprite = LoadTexture("../assets/soul.png");
    std::vector<size_t> marked;

    Rectangle getPlayerHitbox() {
      return  Rectangle {
        .x = this->spatial.pos.x + (this->sprite.width * 0.5f) - (this->in_sprite.width * 0.5f),
        .y = this->spatial.pos.y + (this->sprite.height * 0.5f) - (this->in_sprite.height * 0.5f),
        .width = (float)this->in_sprite.width,
        .height = (float)this->in_sprite.height
      };
    }
  };
}
