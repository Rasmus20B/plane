#pragma once
#include <vector>
#include <raylib.h>

namespace plane {

struct TextureManager {
  std::vector<Texture2D> textures;
};

int storeTexture(TextureManager& tm, const char* path);
void initTextureManager(TextureManager &tm);
}
