#include "spline.h"


#include <iostream> 
namespace plane {

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

    t = t - (int)t;
    float tt = t * t;
    float ttt = tt * t;

    float q1 = -ttt + 2.0f*tt - t;
    float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
    float q3 = -3.0f * ttt + 4.0f * tt + t;
    float q4 = ttt - tt;

    std::cout << "before. p3 :  " << p1 << "\n";
    float tx = 0.5f * (pts[p0].pos.x * q1 + pts[p1].pos.x * q2 + pts[p2].pos.x * q3 + pts[p3].pos.x * q4);
    float ty = 0.5f * (pts[p0].pos.y * q1 + pts[p1].pos.y * q2 + pts[p2].pos.y * q3 + pts[p3].pos.y * q4);
    std::cout << "after\n";

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
      p -= pts[i].len;
      i++;
    }

    return (float)i + (p / pts[i].len);
  }
}



