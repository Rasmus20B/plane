#pragma once
#include "../common/vector_calc.h"
#include "../common/config.h"

#include "texture.h"
#include "collision.h"

#include "raylib.h"

namespace plane {

enum class BulletFlag : uint8_t {
  NORM = 0,
  AIMED = 1,
  RING_AIMED = 2,
  RING_AROUND = 4,
  SIZE 
};

enum class BulletSprite : uint8_t {
  ORB_01 = 0,
  ORB_02 = 1,
  ORB_03 = 2,
  PELLET_01 = 3,
  BLADE_01 = 4,
  S_SIZE
};

struct BulletMgr {

  Texture2D sprite;
  std::vector<Vec2> positions;
  std::vector<bool> oobs;
  Vec2 origin;
  float ang1;
  float ang2;
  float speed1;
  float speed2;
  uint8_t count;
  uint8_t layers;
  BulletFlag mode;
  BulletSprite type;

  Vec2& getPos(const int r, const int c) noexcept;
  void setOrigin(const Vec2 o) noexcept;
  void setCount(const uint16_t l, const uint16_t c) noexcept;
  void setSpeed(const float s1, const float s2) noexcept;
  void setType(const BulletSprite s) noexcept;
  void setAngle(float a1, float a2, Vec2* p ) noexcept;
  void setOutOfBounds() noexcept;
  void update() noexcept;
  void draw() noexcept;
  void drawHitbox() noexcept;
  bool collision_check(const Rectangle& hitbox) noexcept;
  bool OutOfBounds(Vec2 pos, const float width, const float height) noexcept;
};
}
