#pragma once
#include <vector>
#include <raylib.h>

namespace plane {

struct TextureManager {
  std::vector<Texture2D> actSprites;
  std::vector<Texture2D> eBulletSprites;
  std::vector<Texture2D> pBulletSprites;
  std::vector<Texture2D> fx;
};

int storeTexture(TextureManager& tm, const char* path);
void initTextureManager(TextureManager &tm);

inline TextureManager tm;
}
