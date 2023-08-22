#include "texture.h"

namespace plane {

  void initTextureManager(TextureManager &tm) {
    tm.fx.push_back(LoadTexture("../assets/shot.png"));
    tm.eBulletSprites.push_back(LoadTexture("../assets/orb1.png"));
    tm.eBulletSprites.push_back(LoadTexture("../assets/orb2.png"));
    tm.eBulletSprites.push_back(LoadTexture("../assets/orb3.png"));
    tm.eBulletSprites.push_back(LoadTexture("../assets/pellet1.png"));
    tm.eBulletSprites.push_back(LoadTexture("../assets/blade1.png"));
    tm.fx.push_back(LoadTexture("../assets/background_rocks.png"));
    tm.fx.push_back(LoadTexture("../assets/background_water.png"));
    tm.fx.push_back(LoadTexture("../assets/explosion.png"));
    tm.eBulletSprites.push_back(LoadTexture("../assets/enemy1.png"));
    tm.actSprites.push_back(LoadTexture("../assets/enemy1.png"));
    tm.backgrounds.push_back(LoadTexture("../assets/arena.png"));
    
  }
}
