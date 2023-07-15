#include "input.h"

namespace plane {
  int handle_game_input(pSpace& p) {
      if(IsKeyDown(KEY_UP)) {
        p.pos.y -= p.speed;
      }

      if(IsKeyDown(KEY_DOWN)) {
        p.pos.y += p.speed;
      }

      if(IsKeyDown(KEY_RIGHT)) {
        p.pos.x += p.speed;
      }

      if(IsKeyDown(KEY_LEFT)) {
        p.pos.x -= p.speed;
      }

      if(IsKeyDown(KEY_LEFT_SHIFT)) {
        p.micro = true;
        p.speed = 2.5f;
      } else if(IsKeyReleased(KEY_LEFT_SHIFT)) {
        p.micro = false;
        p.speed = 8.0f;
      }

      if(IsKeyDown(KEY_SPACE)) {
        
      }

      return 0;
  }
}
