#include <vector> 

#include <raylib.h> 
#include "planes.h"

#include <stdlib.h> 

#include <iostream>

namespace plane {

  int main_loop() {

    Player p;
    InitWindow(config.screen_width, config.screen_height, "Plane");

    SetTargetFPS(60);

    int score = 0;

    float pos = 0;

    bool ded_flag;
    std::vector<Projectile> p_bullets;
    std::vector<Projectile> e_bullets;
    std::vector<Enemy> enemies;
    p_bullets.reserve(40);
    e_bullets.reserve(200);
    enemies.reserve(100);
    float t_lastShot = GetTime();
    float t_lasteShot = GetTime();
    float t_laste = GetTime();

    enemies.push_back(Enemy{
        .pos = {fdiv(config.screen_width, 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -100}, 0}, {Vector2{2000, -200}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 5.0f, 
        .size = 20, 
        .lt = {0, 11}, 
        .dead = false});
    enemies.push_back(Enemy{
        .pos = {fdiv(config.screen_width, 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -100}, 0}, {Vector2{2000, -200}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 5.0f, 
        .size = 20, 
        .lt = {0.20f, 11}, 
        .dead = false});
    enemies.push_back(Enemy{
        .pos = {fdiv(config.screen_width, 2), 0 }, 
        .mpat{ {{Vector2{10, 400}, 0}, {Vector2{0, 100}, 0}, {Vector2{float(config.screen_width) + 20, 100}, 0}, {Vector2{600, 400}, 0}, {Vector2{800, 40}, 0}, {Vector2{1000, -100}, 0}, {Vector2{2000, -200}, 0} }, 0 },
        .prog = 0,
        .spline_t = 0.01, 
        .shoot_t = 0.7f, 
        .last_shot = 0, 
        .speed = 5.0f, 
        .size = 20, 
        .lt = {0.40f, 11}, 
        .dead = false});
    for(int j = 0; j < enemies.size(); j++) {
        for(int i = 0; i < enemies[i].mpat.pts.size(); i++) {
          enemies[j].mpat.len += (enemies[j].mpat.pts[i].len = enemies[j].mpat.SegmentLength(i, true));
        }
    }
    // enemies.push_back(Enemy{
    //     {fdiv(config.screen_width, 2 ), 20 }, 
    //     {fdiv(config.screen_width, 2), 20}, 
    //     {Vector2{1, 1}, Vector2{20, 2}, Vector2{400, 2}, Vector2{700, 0}}, 
    //     0, 0.7f, 0, 0.02f, 30, {0, 11}, false});
    //
    // enemies.push_back(Enemy{
    //     {fdiv(config.screen_width, 2), 40 }, 
    //     {fdiv(config.screen_width, 2), 40 }, 
    //     {Vector2{1, 1}, Vector2{20, 2}, Vector2{400, 40}, Vector2{800, 3}}, 
    //     0, 0.7f, 0, 0.02f, 20, {0, 11}, false});

    while(!WindowShouldClose() && !ded_flag) {
      if(IsKeyDown(KEY_UP)) {
        p.pos.y -= p.speed;
      }
      if(IsKeyDown(KEY_DOWN)) {
        p.pos.y += p.speed;
      }
      if(IsKeyDown(KEY_RIGHT)) {
        p.pos.x += p.speed;
      }
      if(IsKeyDown(KEY_LEFT)) {
        p.pos.x -= p.speed;
      }

      if(IsKeyDown(KEY_SPACE)) {
        if(GetTime() - t_lastShot > 0.10) {
          Projectile tmp{ Vector2(p.pos), 10, 5.0f};
          p_bullets.push_back(tmp);
          t_lastShot = GetTime();
        }
      }
      if(GetTime() - t_lasteShot > 1) {
        t_lasteShot = GetTime();
      }

      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(p.pos, p.size, RED);
        DrawCircleV(p.pos, p.in_size, WHITE);

        for(auto &e : enemies) {
          if(!e.dead && e.lt.contains(GetTime()) ) {
            if(e.prog >= float(e.mpat.len) - e.speed) e.prog -= e.mpat.len;
            std::cout << e.mpat.len << "\n";
            e.spline_t = e.mpat.getNormalisedOffset(e.prog += e.speed);
            std::cout << e.spline_t << "\n";
            e.pos = e.mpat.getPoint(e.spline_t, false);
            SplinePt tmp = e.mpat.getGradient(e.spline_t, false);

            std::cout << e.pos.pos.x << ", " << e.pos.pos.y << "\n";
            std::cout << tmp.pos.x << ", " << tmp.pos.y << "\n";


            auto angle = Vec2Angle(e.pos.pos, tmp.pos);
            std::cout << angle << "\n";

            if(GetTime() - e.last_shot > e.shoot_t) {
              e_bullets.push_back(Projectile{Vector2{e.pos.pos.x, e.pos.pos.y}, 10, 14}); 
              e.last_shot = GetTime();
            }
            DrawCircleV(e.pos.pos, e.size, ORANGE);
          }
        }
        for(auto &i : p_bullets ) {
          i.pos.y -= i.speed;
          DrawCircleV(i.pos, i.size, BLUE);
          for(auto &e : enemies) {
            if(!e.dead  && CheckCollisionCircles(i.pos, i.size, e.pos.pos, e.size)) {
              e.dead = true;
              score++;
            }
          }
        }
        for(auto &i : e_bullets) {
          i.pos.y += i.speed + i.angle;
          i.pos.x += i.angle;
          DrawCircleV(i.pos, i.size, GREEN);
          if(CheckCollisionCircles(i.pos, i.size, p.pos, p.in_size)) {
            std::cout << "FUCKEN DIED\n";
            ded_flag = true;
          }
        }
        DrawFPS((config.screen_width / 6) * 5, 20);
        DrawText(std::to_string(score).data(), (config.screen_width / 6) * 5, 40, 20, RED);
        DrawText(std::to_string(GetMouseX()).data(), (config.screen_width / 6) * 5, 60, 20, RED);
        DrawText(std::to_string(GetMouseY()).data(), (config.screen_width / 6) * 5, 80, 20, RED);
      EndDrawing();
    }
  

    return 0;
  }
}
