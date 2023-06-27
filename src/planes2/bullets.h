#pragma once
#include "../common/vector_calc.h"

#include "texture.h"

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
  Vec2 origin;
  float ang1;
  float ang2;
  float speed1;
  float speed2;
  uint16_t count;
  uint16_t layers;
  BulletFlag mode;

  Vec2& getPos(const int r, const int c) noexcept {
    int idx = c * layers + r;
    return positions[idx];
  }

  void setOrigin(const Vec2 o) noexcept {
    origin = o;
    switch(mode) {
      case BulletFlag::AIMED:
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() = origin.x() + (cos(RAD(ang1) + RAD(ang2) * j + 1));
            getPos(i, j).y() = origin.y() + (sin(RAD(ang1) + RAD(ang2) * j + 1));
          }
        }
        break;
      case BulletFlag::NORM:
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() = origin.x() + (cos(RAD(ang1) + RAD(ang2) * j + 1));
            getPos(i, j).y() = origin.y() + (sin(RAD(ang1) + RAD(ang2) * j + 1));
          }
        }
        break;
      case BulletFlag::RING_AIMED:
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() = origin.x() + (cos(RAD(360.f / j * i + 1) * j * i) + RAD(ang2) * j + 1);
            getPos(i, j).y() = origin.y() + (sin(RAD(360.f / j * i + 1) * j * i) + RAD(ang2) * j + 1);
          }
        }
        break;
      default:
        break;
      }
  }

  void setCount(const uint16_t l, const uint16_t c) noexcept{
    this->layers = l;
    this->count = c;
    positions.resize(l*c+1);
  }

  void setSpeed(const float s1, const float s2) noexcept{
    this->speed1 = s1;
    this->speed2 = s2;
  }

  void setType(const BulletSprite s) noexcept {
    sprite = tm.eBulletSprites[static_cast<int>(s)];
  }

  void setAngle(float a1, float a2, Vec2* p = nullptr) noexcept {
    float add = 0;
    switch(mode) {
    case BulletFlag::AIMED:
      if(!p) return;
        if(this->count & 1) {
          add = -90 - (a2 * ((int)(count * 0.5f) + 1)) + a2;
        } else {
          add = -90 - (a2 * (count * 0.5f)) + (a2 / (count * 0.5f));
        }
      this->ang1 = (*p - this->origin).norm().face_velocity() + a1 + add;
      this->ang2 = a2;
      break;
    case BulletFlag::NORM:
      this->ang1 = a1;
      this->ang2 = a2;
      break;
    case  BulletFlag::RING_AIMED:
      add = -90 - (a2 * (count / 2.f)) + (a2 / (count / 2.f));
      this->ang1 = (*p - this->origin).norm().face_velocity() + a1 + add;
      this->ang2 = a2;
    default:
      break;
    }
  }

  void update() noexcept {

    switch(mode) {

      case BulletFlag::AIMED:
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() += (cos(RAD(ang1) + RAD(ang2) * j) * lspeed);
            getPos(i, j).y() += (sin(RAD(ang1) + RAD(ang2) * j) * lspeed);
          }
        }
        break;
      case BulletFlag::NORM:
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() += (cos(RAD(-90 + ang1) + RAD(ang2) * j) * lspeed);
            getPos(i, j).y() += (sin(RAD(-90 + ang1) + RAD(ang2) * j) * lspeed);
          }
        }
        break;
      case BulletFlag::RING_AIMED:
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() += (cos(RAD(360.f/ ang1 * j + 1 * i + 1) + (RAD(360.f / ang2) * i + 1 *j + 1)) * lspeed);
            getPos(i, j).y() += (sin(RAD(360.f/ ang1  * j + 1 * i + 1) + (RAD(360.f / ang2) * i + 1 *j + 1)) * lspeed);
          }
        }
        break;
      case BulletFlag::RING_AROUND:
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          for(int j = 0; j < count; ++j) {
            getPos(i, j).y() += (float)(RAD(ang2) + cos(RAD( 360.0f / (j + 1)) ) ) * lspeed;
            getPos(i, j).y() += (float)(RAD(ang2) + sin(RAD( 360.0f / (j + 1)) ) ) * lspeed;
          }
        }
        break;
      default:
        break;
    }
  }

  void draw() noexcept {
    Color cols[4] = { WHITE, RED, GREEN, PURPLE };
    for(int i = 0; i < layers; ++i) {
      for(int j = 0; j < count; ++j) {
        Rectangle ps_hitbox = Rectangle{
                getPos(i, j).vec.x, 
                getPos(i, j).vec.y, 
                static_cast<float>(sprite.width ), 
                static_cast<float>(sprite.height )
        };
        DrawTexturePro(
          this->sprite, 
          { 
            0,
            0,
            static_cast<float>(sprite.width),
            static_cast<float>(sprite.height)
          },
          ps_hitbox,
          {(float)sprite.width / 2 , (float)sprite.height / 2 },
          Vec2(origin - getPos(i, j)).face_velocity(), WHITE);
      }
    }
  }
};
}
