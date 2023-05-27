#include "spline.h"


#include <iostream> 
namespace plane {

  // Vector2 Spline::calculate(float t, bool cont) {
  //   int n = cont ? points.size() : points.size() - 3;
  //   float u = t * n;
  //   int i = (t >= 1) ? (n-1) : (int)u;
  //   u-=i;
  //
  //   return calculate(t, i, u, cont);
  // }
  //
  // Vector2 Spline::calculate(float t, int i, float u, bool cont) {
  //   int n = points.size();
  //   float u2 = u*u;
  //   float u3 = u*u*u;
  //
  //   Vector2 out = Vec2Mul(points[i], 1.5f * u3 - 2.5f * u2 - 1.0f);
  //   if(cont || i > 0) {
  //     out = Vec2Add(out,
  //             Vec2Mul(points[(n + i - 1) % points.size()], -0.5f * u3 + u2 - 0.5f * u));
  //   }
  //   if(cont || i < (n - 1)) {
  //     out = Vec2Add(out, Vec2Mul(points[(i + 1) % points.size()], -1.5f * u3 + 2.0f * u2 + 0.5f * u));
  //   }
  //   if (cont || i < (n - 2)) {
  //     out = Vec2Add(out, Vec2Mul(points[(i + 2) % points.size()], 0.5f * u3 - 0.5f * u2));
  //   }
  //   return out;
  // }
  // Vector2 Spline::interpolate(float t, float alpha) {
  //   
  //   if(t >= 1.0f) {
  //     std::cout << "reached end of curve\n";
  //     return {};
  //   }
  //   float tension = 0;
  //
  //   float t01 = sqrt(pow(points[0].x, 2) - pow(points[1].x, 2) +
  //                    pow(points[0].y, 2) - pow(points[1].y, 2));
  //   float t02 = sqrt(pow(points[1].x, 2) - pow(points[2].x, 2) +
  //                    pow(points[1].y, 2) - pow(points[2].y, 2));
  //   float t03 = sqrt(pow(points[2].x, 2) - pow(points[2].x, 2) +
  //                    pow(points[3].y, 2) - pow(points[3].y, 2));
  //
  //   Vector2 m1 = Vec2Mul(
  //       Vec2Sub(Vec2Div(Vec2Sub(points[1], points[0]), t01 - 0),
  //               Vec2Add(Vec2Div(Vec2Sub(points[2], points[0]), t02 - 0),
  //                       Vec2Div(Vec2Sub(points[2], points[1]), (t02 - t01)))),
  //       (1 - tension) * (t02 - t01));
  //
  //   Vector2 m2 = Vec2Mul(
  //       Vec2Sub(Vec2Div(Vec2Sub(points[2], points[1]), t02 - t01),
  //               Vec2Add(Vec2Div(Vec2Sub(points[3], points[1]), t03 - t01),
  //                       Vec2Div(Vec2Sub(points[3], points[2]), (t03 - t02)))),
  //       (1 - tension) * (t03 - t02));
  //
  //   Spline n;
  //   n.points.resize(4);
  //   n.points[0] =
  //       Vec2Add(Vec2Add(Vec2Mul(Vec2Sub(points[1], points[2]), 2), m1), m2);
  //   n.points[1] = Vec2Sub(
  //       Vec2Sub(Vec2Sub(Vec2Mul(Vec2Sub(points[1], points[2]), -3), m1), m1),
  //       m2);
  //   n.points[2] = m1;
  //   n.points[3] = points[1];
  //   std::cout << "gets here\n";
  //
  //   return {
  //     Vec2Add(Vec2Mul(Vec2Mul(Vec2Mul(points[0], t), t), t),
  //           Vec2Add(Vec2Mul(Vec2Mul(points[1], t), t),
  //             Vec2Add(Vec2Mul(points[2], t),
  //               points[3])))
  //   };
  // }
  //
  // Vector2 Spline::getDerivative(float t, bool continuous) {
  //   int n = continuous ? points.size() : points.size() - 3;
  //   float u = t * n;
  //   int i = (t >= 1) ? (n - 1) : (int)u;
  //   u -= i;
  //
  //   std::cout << i << "\n";
  //
  //   return getDerivative(t, i, u, continuous);
  // }
  //
  // Vector2 Spline::getDerivative(float t, int i, float u, bool continuous) {
  //   Vector2 out;
  //   int n = points.size();
  //
  //   out = Vec2Mul(points[i], -u * 5 + (u*u) * 4.5f);
  //   if(continuous || i > 0) {
  //     std::cout << "gets here i > 0\n";
  //     out = Vec2Add(out,
  //             Vec2Mul(points[(n + i - 1) % points.size()], -0.5f + u * 2 - (u * u) * 1.5f));
  //   }
  //   if(continuous || i < (n - 1)) {
  //     std::cout << "gets here n - 1\n";
  //     out = Vec2Add(out, Vec2Mul(points[(i + 1) % points.size()], 0.5f + u * 4 - (u * u) * 4.5f));
  //   }
  //   if (continuous || i < (n - 2)) {
  //     std::cout << "gets here n - 2\n";
  //     out = Vec2Add(out, Vec2Mul(points[(i + 2) % points.size()], -u + (u * u) * 1.5f));
  //   }
  //
  //   return out;
  // }

  SplinePt Spline::getGradient(float t, bool looped) {
    int p0, p1, p2, p3;

    if(looped) {
      p1 = (int)t;
      p2 = (p1 + 1) % pts.size();
      p3 = (p2 + 1) % pts.size();
      p0 = p1 >= 1 ? p1 - 1 : pts.size() - 1;
    } else {
      p1 = (int)t + 1;
      p2 = p1 + 1;
      p3 = p2 + 1;
      p0 = p1 - 1;
    }

    t = t - (int)t;
    float tt = t * t;
    float ttt = tt * t;

    float q1 = -3.0f * tt + 4.0f*t - 1;
    float q2 = 9.0f * tt - 10.0f * t;
    float q3 = -9.0f * tt + 8.0f * t + 1.0f;
    float q4 = 3.0f*tt - 2.0f*t;

    float tx = 0.5f * (pts[p0].pos.x * q1 + pts[p1].pos.x * q2 + pts[p2].pos.x * q3 + pts[p3].pos.x * q4);
    float ty = 0.5f * (pts[p0].pos.y * q1 + pts[p1].pos.y * q2 + pts[p2].pos.y * q3 + pts[p3].pos.y * q4);
    return { tx, ty };
  }
  SplinePt Spline::getPoint(float t, bool looped = false) {
    int p0, p1, p2, p3;

    if(looped) {
      p1 = (int)t;
      p2 = (p1 + 1) % pts.size();
      p3 = (p2 + 1) % pts.size();
      p0 = p1 >= 1 ? p1 - 1 : pts.size() - 1;
    } else {
      p1 = (int)t + 1;
      p2 = p1 + 1;
      p3 = p2 + 1;
      p0 = p1 - 1;
    }

    std::cout << "here\n";

    t = t - (int)t;
    float tt = t * t;
    float ttt = tt * t;

    float q1 = -ttt + 2.0f*tt - t;
    float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
    float q3 = -3.0f * ttt + 4.0f * tt + t;
    float q4 = ttt - tt;

    std::cout << pts[p2].pos.x << "\n";
    std::cout << "there \n";
    float tx = 0.5f * (pts[p0].pos.x * q1 + pts[p1].pos.x * q2 + pts[p2].pos.x * q3 + pts[p3].pos.x * q4);
    float ty = 0.5f * (pts[p0].pos.y * q1 + pts[p1].pos.y * q2 + pts[p2].pos.y * q3 + pts[p3].pos.y * q4);

    std::cout << "everywhere \n";
    return { tx, ty };
  }

  float Spline::SegmentLength(int node, bool looped = false) {
    float flength = 0.0f;
    float fStep = 0.005;

    SplinePt old, cur;
    old = getPoint((float)node, true);

    for(float t = 0; t < 1.0f; t += fStep) {
      cur = getPoint((float)node + t, true);
      flength += sqrtf(pow(cur.pos.x - old.pos.x, 2) + pow(cur.pos.y - old.pos.y, 2));
      old = cur;
    }

    return flength;
  }

  float Spline::getNormalisedOffset(float p) {
    int i = 0;
    while((float)p > pts[i].len) {
      std::cout << "looking at point : " << i << "with len " << pts[i].len << "\n";
      p -= pts[i].len;
      i++;
    }
      std::cout << "scapes loop : " << i << '\n';

    return (float)i + (p / pts[i].len);
  }
}



