#include "../src/planes2/collision.h"
#include <raylib.h>
#include <gtest/gtest.h>

#include <chrono>

using namespace plane;

TEST(collision, getCorners) {
  Rectangle r = { 200, 200, 300, 400 };

  // Scaler Benchmark
  std::cout << "SCALER::::::::::::::::::::::::::\n";
  auto s = std::chrono::steady_clock::now().time_since_epoch();
  std::array<Vec2, 4> res1 = getCorners(r, 0.f);
  auto res = std::chrono::steady_clock::now().time_since_epoch() - s;
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(res) << "\n";

  // SIMD Benchmark
  std::cout << "VECTOR::::::::::::::::::::::::::\n";
  s = std::chrono::steady_clock::now().time_since_epoch();
  std::array<Vec2, 4> res2 = getCornersSIMD(r, 0.f);
  res = std::chrono::steady_clock::now().time_since_epoch() - s;
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(res) << "\n";

  for(int i = 0; i < res1.size(); ++i) {
    EXPECT_EQ(res1[i], res2[i]);
  }
}

TEST(collision, checkContains) {
  auto cs1 = getCorners({
      200, 200, 300, 400
      }, 0);
  auto cs2 = getCorners({
      300, 200, 300, 400
      }, 0);

  // Scaler Benchmark
  std::cout << "SCALER::::::::::::::::::::::::::\n";
  auto s = std::chrono::steady_clock::now().time_since_epoch();
  auto res1 = checkContains(cs1, cs2);
  auto res = std::chrono::steady_clock::now().time_since_epoch() - s;
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(res) << "\n";

  // SIMD BenchMark
  std::cout << "VECTOR::::::::::::::::::::::::::\n";
  s = std::chrono::steady_clock::now().time_since_epoch();
  auto res2 = checkContainsSIMD(cs1, cs2);
  res = std::chrono::steady_clock::now().time_since_epoch() - s;
  std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(res) << "\n";

  EXPECT_EQ(res1, res2);
}
