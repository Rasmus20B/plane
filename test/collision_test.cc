#include "../src/planes2/collision.h"
#include <raylib.h>
#include <gtest/gtest.h>

#include <chrono>


using namespace plane;

TEST(collision, getCorners) {
  Rectangle r = { 200, 200, 300, 400 };

  // Scaler Benchmark
  auto s = std::chrono::steady_clock::now().time_since_epoch().count();
  std::array<Vec2, 4> res1 = getCorners(r, 0.f);
  auto res = std::chrono::steady_clock::now().time_since_epoch().count() - s;
  std::cout << res << "\n";

  // SIMD Benchmark
  s = std::chrono::steady_clock::now().time_since_epoch().count();
  std::array<Vec2, 4> res2 = getCornersSIMD(r, 0.f);
  res = std::chrono::steady_clock::now().time_since_epoch().count() - s;
  std::cout << res << "\n";

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
  auto s = std::chrono::steady_clock::now().time_since_epoch().count();
  auto res1 = checkContains(cs1, cs2);
  auto res = std::chrono::steady_clock::now().time_since_epoch().count() - s;

  std::cout << res1 << " : " << res << "\n";

  // SIMD BenchMark
  s = std::chrono::steady_clock::now().time_since_epoch().count();
  auto res2 = checkContains(cs1, cs2);
  res = std::chrono::steady_clock::now().time_since_epoch().count() - s;

  EXPECT_EQ(res1, res2);
  std::cout << res2 << " : " << res << "\n";
}
