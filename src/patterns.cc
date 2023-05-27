
#include "patterns.h"

namespace  plane {
Vector2 ltr_pattern(Vector2 cur, Vector2& prev) {
  Vector2 newpos;
  newpos.y = cur.y;
  if((cur.x >= config.screen_width || prev.x >= cur.x) &&  cur.x > 0) {
    newpos.x = cur.x - 3;
  } else {
    newpos.x = cur.x + 3; 
  }
  prev = cur;
  return newpos;
}

Vector2 spline_test(Vector2 cur, Vector2& prev) {
  Spline s;
  // s.points[0] = {20, 20};
  // s.points[1] = {40, 40};
  // s.points[2] = {60, 60};
  // s.points[3] = {80, 80};
  Vector2 newpos;

  return newpos;
}

}
