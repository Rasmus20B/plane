#include "bullets.h"

namespace plane {

  void BulletMgr::init() noexcept {
  }
  Vec2& BulletMgr::getPos(const int r, const int c) noexcept {
    int idx = c * layers + r;
    return positions[idx];
  }

  void BulletMgr::setAim(BulletFlag bf) noexcept {
    this->mode = bf;
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
        {
        float angstep = 360.f / (count);
        float angle = 0;
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() = origin.x() + sin(RAD(angle)) * 2;
            getPos(i, j).y() = origin.y() + cos(RAD(angle)) * 2;
          }
          angle += angstep;
        }
        break;
        }
      case BulletFlag::RING_AROUND:
        {
        float angstep = 360.f / (count);
        float angle = 0;
        for(int i = 0; i < layers; ++i) {
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() = origin.x() + sin(RAD(angle)) * 2;
            getPos(i, j).y() = origin.y() + cos(RAD(angle)) * 2;
          }
          angle += angstep;
        }
        break;
        }
      default:
        break;
      }
  }

  void BulletMgr::setCount(const uint16_t l, const uint16_t c) noexcept{
    this->layers = l;
    this->count = c;
    positions.resize(l*c);

    oobs.resize(l*c);
    /* set last element of out of bounds to true to allow adjacent find later */
    /* using last element because it will be ahead of others and won't cause 
     * visible despawn */
    oobs[oobs.size()-1] = true;
  }

  void BulletMgr::setSpeed(const float s1, const float s2) noexcept{
    this->speed1 = s1;
    this->speed2 = s2;
  }

  void BulletMgr::setType(const BulletSprite s) noexcept {
    sprite = tm.eBulletSprites[static_cast<int>(s)];
    type = s;
  }

  void BulletMgr::setOutOfBounds() noexcept {
    for(int i = 0; i < layers; ++i) {
      for(int j = 0; j < count; ++j) {
        if(OutOfBounds(getPos(i, j), config.screen_width, config.screen_height)) {
          this->oobs[j * layers + i] = true;
        }
      }
    }
  }

  void BulletMgr::setAngle(const float a1, const float a2) noexcept {
    float add = 0;
    switch(mode) {
    case BulletFlag::AIMED:
      if(this->count & 1) {
        add = -90 - (a2 * ((int)(count * 0.5f) + 1)) + a2;
      } else {
        add = -90 - (a2 * (count * 0.5f)) + (a2 / (count * 0.5f));
      }
      this->ang1 = a1 + add;
      this->ang2 = a2;
      break;
    case BulletFlag::NORM:
      this->ang1 = a1;
      this->ang2 = a2;
      break;
    case  BulletFlag::RING_AIMED:
      add = -90 - (a2 * (count / 2.f)) + (a2 / (count / 2.f));
      this->ang1 = a1 + add;
      this->ang2 = a2;
    default:
      break;
    }
  }

  void BulletMgr::update() noexcept {
    switch(mode) {
      case BulletFlag::AIMED:
        for(int i = 0; i < layers; ++i) {
          // For regular aimed managers, the speeds will be calculated between sp1 and sp2, 
          // decreasing to sp2 based on current layer
          float lspeed = (speed1 + speed2) + ((speed1 - speed2) / (float(layers) / (i )));
          for(int j = 0; j < count; ++j) {
            Vec2 *pos = &getPos(i, j);
            pos->x() += (cos(RAD(ang1) + RAD(ang2) * j) * lspeed);
            pos->y() += (sin(RAD(ang1) + RAD(ang2) * j) * lspeed);
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
        {
        const float angstep = 360.f / count;
        float a1 = ang1;
        float a2 = 0;
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          const float r = sqrt(std::pow(getPos(i, 0).x() - origin.x() + lspeed, 2) + std::pow(getPos(i, 0).y() - origin.y() + lspeed, 2)) ;
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() = origin.x() + cos(RAD( a1 )) * r;
            getPos(i, j).y() = origin.y() + sin(RAD( a1 )) * r;
            a1 += angstep;
          }
          a1 = ang1;
          a2 += ang2;
        }
        break;
        }
      case BulletFlag::RING_AROUND:
        {
        const float angstep = 360.f / count;
        float angle = 0;
        for(int i = 0; i < layers; ++i) {
          float lspeed = (speed1 + speed2) / (float(layers + 1) / (i + 1));
          const float r = sqrt(std::pow(getPos(i, 0).x() - origin.x() + lspeed, 2) + std::pow(getPos(i, 0).y() - origin.y() + lspeed, 2)) ;
          for(int j = 0; j < count; ++j) {
            getPos(i, j).x() = origin.x() + cos(RAD( angle )) * r;
            getPos(i, j).y() = origin.y() + sin(RAD( angle )) * r;
            angle += angstep;
          }
          angle = 0;
        }
        break;
        }
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
#ifdef DRAW_HITBOX
        this->drawHitbox();
#endif
  }

  bool BulletMgr::collision_check(const Rectangle& hitbox) noexcept {
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
            if(CheckCollisionCircleRec(getPos(i, j).vec, sprite.width * 0.5, hitbox)) {
              return true;
            }
          }
        }
        break;
      default:
        break;
    }
    return false;
  }

  bool BulletMgr::OutOfBounds(Vec2 pos, const float width, const float height) noexcept {
    if(pos.x() > width || pos.y() > height || pos.x() < 0 || pos.y() < 0) {
      return true;
    }
    return false;
  }

  void BulletMgr::shoot(Vec2 esp, Vec2 tar) noexcept {
    this->setOrigin(esp);
    switch(this->mode) {
      case BulletFlag::AIMED:
        ang1 += (tar - this->origin).norm().face_velocity();
        break;
      case BulletFlag::RING_AIMED:
        ang1 += (tar - this->origin).norm().face_velocity();
        break;
      default:
        break;
    }
  }
}
