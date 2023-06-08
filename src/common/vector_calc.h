#pragma once

#include <raylib.h>

#include <cmath>

template<typename T>
struct VecBase {
  static constexpr size_t size() {
    return T::size;
  }
};

struct Vec2 : public VecBase<Vec2> {
  Vec2(const float c_x, const float c_y) : vec{c_x, c_y} {};

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
  float magnitude() {
    return sqrtf(pow(this->vec.x, 2) + pow(this->vec.y, 2));
  }
  float dot(const Vec2& rhs) {
    return (this->vec.x * rhs.vec.x) + (this->vec.y * rhs.vec.y);
  }
  Vec2 norm() {
    return { this->vec.x / this->magnitude(), this->vec.y / this->magnitude() };
  }

  float angle(Vec2& rhs) {
    return acos(this->dot(rhs) / ((this->magnitude()) * (rhs.magnitude())));
  }
  Vector2 vec;
};

struct Vec3 : public VecBase<Vec3> {

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
