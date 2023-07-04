#include "bullets.h"

namespace plane {

  Vec2& BulletMgr::getPos(const int r, const int c) noexcept {
    int idx = c * layers + r;
    return positions[idx];
  }

  void BulletMgr::setOrigin(const Vec2 o) noexcept {
    origin = o;
    switch(mode) {
      case BulletFlag::AIMED:
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

  void BulletMgr::setCount(const uint16_t l, const uint16_t c) noexcept{
    this->layers = l;
    this->count = c;
    positions.resize(l*c+1);
  }

  void BulletMgr::setSpeed(const float s1, const float s2) noexcept{
    this->speed1 = s1;
    this->speed2 = s2;
  }

  void BulletMgr::setType(const BulletSprite s) noexcept {
    sprite = tm.eBulletSprites[static_cast<int>(s)];
    type = s;
  }

  void BulletMgr::setAngle(float a1, float a2, Vec2* p = nullptr) noexcept {
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

  void BulletMgr::update() noexcept {

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

  void BulletMgr::drawHitbox() noexcept {

    for(int j = 0; j < this->layers; ++j) {
      for(int k = 0; k < this->count; ++k) {
        auto coord = this->getPos(j, k);
        Rectangle bhitbox = {
          coord.x(),
          coord.y(),
          (float)this->sprite.width,
          (float)this->sprite.height
        };
        DrawRectanglePro(bhitbox, 
            {this->sprite.width * 0.5f, this->sprite.height * 0.5f},
            Vec2(this->origin - this->getPos(j, k)).face_velocity(), 
            GetColor(0xff000088));
      }
    }
    return;
  }

  void BulletMgr::draw() noexcept {
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

  bool BulletMgr::collision_check(const Rectangle& hitbox) {
    bool hit = false;
    switch(this->type) {
      case BulletSprite::PELLET_01:
      case BulletSprite::BLADE_01:
      case BulletSprite::ORB_02:
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            Rectangle ps_hitbox = Rectangle{
                    getPos(i, j).vec.x, 
                    getPos(i, j).vec.y, 
                    static_cast<float>(sprite.width), 
                    static_cast<float>(sprite.height)
            };
            if(CheckCollisionRecsAngle(
                  hitbox,
                  RAD(0.f),
                  ps_hitbox,
                  RAD(Vec2(origin - getPos(i, j)).face_velocity())
                  )) {
              return true;
            }
          }
        }
        break;
      case BulletSprite::ORB_01:
      case BulletSprite::ORB_03:
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            if(CheckCollisionCircleRec(getPos(i, j).vec, sprite.width * 0.5, hitbox)) 
              return true;
          }
        }
      default:
        break;
    }
    return hit;
  }
}
