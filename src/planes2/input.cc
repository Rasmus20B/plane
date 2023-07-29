#include "input.h"
#include "player.h"
#include <raylib.h>

namespace plane {
  int handle_game_input(pSpace& p, pShooting& s) {
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

      if(IsKeyDown(KEY_Z)) {
        if (GetTime() - s.lastshot > s.shottime) {
          //shoot
          s.lastshot = GetTime();
        }
      }

      return 0;
  }
}
