#pragma once
#include <vector> 
#include <cmath>
#include <raylib.h>

#include "vector_calc.h"

namespace plane {

struct SplinePt {
  Vector2 pos;
  float len;
};
struct Spline {
  void init();
  SplinePt getPoint(float t, bool looped);
  SplinePt getGradient(float t, bool looped = false);
  float SegmentLength(int node, bool looped);
  float getNormalisedOffset(float p);
  
  //
  // Vector2 interpolate(float t, float alpha = 1.0f);
  // Vector2 getDerivative(float t, bool continuous);
  // Vector2 getDerivative(float t, int i, float u, bool continuous);
  // Vector2 calculate(float t, bool cont);
  // Vector2 calculate(float t, int i, float u, bool cont);
  //
  // std::vector<Vector2> points;
  std::vector<SplinePt> pts;
  float len;
};
}
