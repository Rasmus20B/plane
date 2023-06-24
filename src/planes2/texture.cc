#include "texture.h"

namespace plane {
  int storeTexture(TextureManager& tm, const char* path) {
    tm.textures.push_back(LoadTexture(path));
    return 0;
  }

  void initTextureManager(TextureManager &tm) {
    tm.textures.push_back(LoadTexture("../assets/player.png"));
    tm.textures.push_back(LoadTexture("../assets/shot.png"));
    tm.textures.push_back(LoadTexture("../assets/soul.png"));
    tm.textures.push_back(LoadTexture("../assets/orb1.png"));
    tm.textures.push_back(LoadTexture("../assets/orb2.png"));
    tm.textures.push_back(LoadTexture("../assets/pellet1.png"));
    tm.textures.push_back(LoadTexture("../assets/background_rocks.png"));
    tm.textures.push_back(LoadTexture("../assets/background_water.png"));
    tm.textures.push_back(LoadTexture("../assets/enemy1.png"));
    tm.textures.push_back(LoadTexture("../assets/orb3.png"));
    tm.textures.push_back(LoadTexture("../assets/explosion.png"));
  }
}
