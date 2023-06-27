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

template<uint16_t N, uint16_t L>
struct BulletMgr {
  Texture2D sprite;
  Vec2 positions[L][N];
  Vec2 origin;
  float ang1;
  float ang2;
  float speed1;
  float speed2;
  uint32_t entry_frame;
  uint16_t count = N;
  uint16_t layers = L;
  BulletFlag mode;

  void setOrigin(const Vec2 o) {
    origin = o;
    switch(mode) {
      case BulletFlag::AIMED:
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            positions[i][j].x() = origin.x() + (cos(RAD(ang1) + RAD(ang2) * j + 1));
            positions[i][j].y() = origin.y() + (sin(RAD(ang1) + RAD(ang2) * j + 1));
          }
        }
        break;
      case BulletFlag::RING_AIMED:
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
                  // enemies.space[i].position.vec.x + (float)(15 * cos((360.0f/s->second.size) * j)),
                  // enemies.space[i].position.vec.y + (float)(15 * sin((360.0f/s->second.size) * j))
            positions[i][j].x() = origin.x() + (cos(RAD(360.f / j * i + 1) * j * i) + RAD(ang2) * j + 1);
            positions[i][j].y() = origin.y() + (sin(RAD(360.f / j * i + 1) * j * i) + RAD(ang2) * j + 1);
          }
        }
        break;
      }
      
  }

  void setSpeed(const float s1, const float s2) {
    this->speed1 = s1;
    this->speed2 = s2;
  }

  void setType(const BulletSprite s) {
    sprite = tm.eBulletSprites[static_cast<int>(s)];
  }

  void setAngle(float a1, float a2, Vec2* p = nullptr) {
    if(mode == BulletFlag::AIMED) {
      if(!p) return;
      float add = 0;
        if(this->count & 1) {
          add = -90 - (a2 * ((int)(count * 0.5f) + 1)) + a2;
        } else {
          add = -90 - (a2 * (count * 0.5f)) + (a2 / (count * 0.5f));
        }
      this->ang1 = (*p - this->origin).norm().face_velocity() + a1 + add;
      this->ang2 = a2;
    } else if(mode == BulletFlag::RING_AIMED) {
      float add = -90 - (a2 * (count / 2.f)) + (a2 / (count / 2.f));
      this->ang1 = (*p - this->origin).norm().face_velocity() + a1 + add;
      this->ang2 = a2;
    }
  }

  void update() {

    switch(mode) {
      case BulletFlag::AIMED:
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          for(int j = 0; j < count; ++j) {
            positions[i][j].vec.x += (cos(RAD(ang1) + RAD(ang2) * j) * lspeed);
            positions[i][j].vec.y += (sin(RAD(ang1) + RAD(ang2) * j) * lspeed);
          }
        }
        break;
      case BulletFlag::RING_AIMED:
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          for(int j = 0; j < count; ++j) {
            positions[i][j].vec.x += (cos(RAD(360.f/ ang1 * j + 1 * i + 1) + (RAD(360.f / ang2) * i + 1 *j + 1)) * lspeed);
            positions[i][j].vec.y += (sin(RAD(360.f/ ang1  * j + 1 * i + 1) + (RAD(360.f / ang2) * i + 1 *j + 1)) * lspeed);
          }
        }
        break;
      case BulletFlag::RING_AROUND:
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          for(int j = 0; j < count; ++j) {
            positions[i][j].vec.x += (float)(RAD(ang2) + cos(RAD( 360.0f / (j + 1)) ) ) * lspeed;
            positions[i][j].vec.y += (float)(RAD(ang2) + sin(RAD( 360.0f / (j + 1)) ) ) * lspeed;
          }
        }
        break;
    }
  }

  void draw() {
    Color cols[4] = { WHITE, RED, GREEN, PURPLE };
    for(int i = 0; i < L; ++i) {
      for(int j = 0; j < N; ++j) {
          Rectangle ps_hitbox = Rectangle{
                  positions[i][j].vec.x, 
                  positions[i][j].vec.y, 
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
            Vec2(origin - positions[i][j]).face_velocity(), WHITE);
      }
    }

    DrawLineV(positions[0][0].vec, positions[3][0].vec, RED);
    DrawLineV(positions[0][1].vec, positions[3][1].vec, RED);
    DrawLineV(positions[0][2].vec, positions[3][2].vec, RED);
    DrawLineV(positions[0][3].vec, positions[3][3].vec, RED);
  }
};



}
