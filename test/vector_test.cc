#include <gtest/gtest.h>

#include "../src/vector_calc.h"

TEST(vector, vector_addition) {
  Vec2 a {5, 6};
  Vec2 b {3, 4};
  EXPECT_EQ(Vec2(8, 10), a + b);
}

TEST(vector, vector_subtraction) {
  Vec2 a {5, 6};
  Vec2 b {3, 4};
  EXPECT_EQ(Vec2(2, 2), a - b);
}

TEST(vector, vector_multiplication_float) {
  Vec2 a {5, 6};
  EXPECT_EQ(Vec2(15, 18), a * 3);
}

TEST(vector, vector_division) {
  Vec2 a {12, 6};
  EXPECT_EQ(Vec2(6, 3), a/2);
}

TEST(vector, vector_dot) {
  Vec2 a {5, 6};
  Vec2 b {3, 4};
  EXPECT_EQ(15+24, a.dot(b));
}

TEST(vector, vector_magnitude) {
  Vec2 a {5, 7};
  EXPECT_EQ(round(sqrt(67) / 100) * 100, round(a.magnitude() / 100) * 100);
}

TEST(vector, vector_norm) {
  Vec2 a { 5, 7 };
  float mag = a.magnitude();
  EXPECT_EQ(Vec2(a.vec.x / mag, a.vec.y / mag), a.norm());
}

TEST(vector, vector_angle) {
  Vec2 a { 5, 7 };
  Vec2 b { 3, 4 };
  EXPECT_EQ(0.023f, (float)round(a.angle(b) * 1000) / 1000);

}
