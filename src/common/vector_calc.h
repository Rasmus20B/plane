#pragma once

#include <raylib.h>

#include <cmath>
#include <iostream>

#define RAD(x) (x * PI) / 180


struct Vec2 {
  Vec2(const float c_x, const float c_y) : vec{c_x, c_y} {};
  Vec2(const Vector2 v) : vec{v.x, v.y} {};
  Vec2() : vec{0, 0}{};

  bool const operator==(const Vec2& rhs) const {
    return (this->vec.x == rhs.vec.x && this->vec.y == rhs.vec.y);
  }
  Vec2 operator+(const Vec2 rhs) {
    return { this->vec.x + rhs.vec.x, this->vec.y + rhs.vec.y };
  }
  Vec2 operator-(const Vec2 rhs) {
    return { this->vec.x - rhs.vec.x, this->vec.y - rhs.vec.y };
  }
  Vec2 operator/(const float divisor) {
    return {fabsf(this->vec.x) / divisor, fabsf(this->vec.y) / divisor};
  }
  Vec2 operator*(const float multiplier) {
    return {this->vec.x * multiplier, this->vec.y * multiplier};
  }
  Vec2 operator*(const Vec2 rhs) {
    return {this->vec.x * rhs.vec.x, this->vec.y * rhs.vec.y};
  }
  float magnitude() {
    return sqrtf(pow(this->vec.x, 2) + pow(this->vec.y, 2));
  }
  float dot(const Vec2& rhs) {
    return (this->vec.x * rhs.vec.x) + (this->vec.y * rhs.vec.y);
  }
  Vec2 norm() {
    return { this->vec.x / this->magnitude(), this->vec.y / this->magnitude() };
  }

  Vec2 rotate(const float angle) const {
    return { this->vec.x * (float)cos(angle) - (this->vec.y * (float)sin(angle)),
             this->vec.x * (float)sin(angle) + (this->vec.y * (float)cos(angle))};
  }

  Vec2 rotate_center(const Vec2 c, const float angle) const {
    return { c.vec.x + ((this->vec.x - c.vec.x) * (float)cos(angle)) + ((this->vec.y - c.vec.y) * (float)sin(angle)),
             c.vec.y - ((this->vec.x - c.vec.y) * (float)sin(angle)) + ((this->vec.y - c.vec.y) * (float)cos(angle))};
  }

  float face_velocity() {
    return atan2(this->vec.x - Vec2{0, 0}.vec.x, Vec2{0, 0}.vec.y - this->vec.y) * (180 / PI); 

  }

  float angle(Vec2 rhs) {
    return acos(this->dot(rhs) / ((this->magnitude()) * (rhs.magnitude())));
  }

  [[nodiscard]]
  float &x() { return vec.x; };
  [[nodiscard]]
  float &y() { return vec.y; };

  Vector2 vec;
};


namespace plane {
Vector2 Vec2Sub(Vector2 a, Vector2 b);
Vector2 Vec2Add(Vector2 a,  Vector2 b);
Vector2 Vec2Div(Vector2 a, float b);
Vector2 Vec2Mul(Vector2 a,  Vector2 b);
Vector2 Vec2Mul(Vector2 a, float b);
float Vec2Dot(Vector2 a, Vector2 b);
float Vec2Angle(Vector2 a, Vector2 b);
Vector2 Vec2len(Vector2 a);
Vector2 Vec2Norm(Vector2 a);
}
