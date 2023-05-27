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


    enemies.push_back(Enemy{{ fdiv(config.screen_width, 2 + 1), 0 }, 
        {fdiv(config.screen_width, 2), 0}, 
        {Vector2{0, 0}, Vector2{30, 2}, Vector2{10, 1}, Vector2{0, 3}}, 
        0, 0.7f, 0, 0.02f, false});

    while(!WindowShouldClose() && !ded_flag) {
      if(IsKeyDown(KEY_UP)) {
        p.pos.y -= 2.0f;
      }
      if(IsKeyDown(KEY_DOWN)) {
        p.pos.y += 2.0f;
      }
      if(IsKeyDown(KEY_RIGHT)) {
        p.pos.x += 2.0f;
      }
      if(IsKeyDown(KEY_LEFT)) {
        p.pos.x -= 2.0f;
      }

      if(IsKeyDown(KEY_SPACE)) {
        if(GetTime() - t_lastShot > 0.10) {
          Projectile tmp{ Vector2(p.pos)};
          p_bullets.push_back(tmp);
          t_lastShot = GetTime();
        }
      }
      if(GetTime() - t_lasteShot > 1) {
        t_lasteShot = GetTime();
      }

      BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(p.pos, 20, RED);
        DrawCircleV(p.pos, 5, WHITE);

        for(auto &e : enemies) {
          if(!e.dead) {
            e.pos = e.mpat.Interpolate(e.spline_t += e.speed);
            std::cout << e.pos.x << ", " << e.pos.y <<"\n";
            if(GetTime() - e.last_shot > e.shoot_t) {
              e_bullets.push_back(Projectile{Vector2{e.pos.x, e.pos.y}});
              e.last_shot = GetTime();
            }
            DrawCircleV(e.pos, 15, ORANGE);
          }
        }
        for(auto &i : p_bullets ) {
          i.pos.y -= 5.0f;
          DrawCircleV(i.pos, 3.0f, BLUE);
          for(auto &e : enemies) {
            if(CheckCollisionCircles(i.pos, 3.0f, e.pos, 5.0f)) {
              e.dead = true;
            }
          }
        }
        for(auto &i : e_bullets) {
          i.pos.y += 5.0f;
          DrawCircleV(i.pos, 3.0f, GREEN);
          if(CheckCollisionCircles(i.pos, 3.0, p.pos, 3.0)) {
            std::cout << "FUCKEN DIED\n";
            ded_flag = true;
          }
        }
      EndDrawing();
    }
  

    return 0;
  }
}
