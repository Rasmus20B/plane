#include "spline.h"


namespace plane {

  Vector2 Spline::Interpolate(float t, float alpha) {
    
    float tension = 0;

    float t01 = sqrt(pow(points[0].x, 2) - pow(points[1].x, 2) + pow(points[0].y, 2) - pow(points[1].y, 2));
    float t02 = sqrt(pow(points[1].x, 2) - pow(points[2].x, 2) + pow(points[1].y, 2) - pow(points[2].y, 2));
    float t03 = sqrt(pow(points[2].x, 2) - pow(points[2].x, 2) + pow(points[3].y, 2) - pow(points[3].y, 2));

    Vector2 m1 = Vec2Mul(
        Vec2Sub(Vec2Div(Vec2Sub(points[1], points[0]), t01 - 0),
                Vec2Add(Vec2Div(Vec2Sub(points[2], points[0]), t02 - 0),
                        Vec2Div(Vec2Sub(points[2], points[1]), (t02 - t01)))),
        (1 - tension) * (t02 - t01));
    
    Vector2 m2 = Vec2Mul(
        Vec2Sub(Vec2Div(Vec2Sub(points[2], points[1]), t02 - t01),
                Vec2Add(Vec2Div(Vec2Sub(points[3], points[1]), t03 - t01),
                        Vec2Div(Vec2Sub(points[3], points[2]), (t03 - t02)))),
        (1 - tension) * (t03 - t02));

    Spline n;
    n.points[0] = Vec2Add(Vec2Add(Vec2Mul(Vec2Sub(points[1], points[2]), 2), m1), m2);
    n.points[1] = Vec2Sub(Vec2Sub(Vec2Sub(Vec2Mul(Vec2Sub(points[1], points[2]), -3), m1), m1), m2);
    n.points[2] = m1;
    n.points[3] = points[1];

    return {
      Vec2Add(Vec2Mul(Vec2Mul(Vec2Mul(points[0], t), t), t),
            Vec2Add(Vec2Mul(Vec2Mul(points[1], t), t),
              Vec2Add(Vec2Mul(points[2], t),
                points[3])))
    };

  }

  Vector2 Spline::getPoint(Vector2) {

    return {};
  }
}
