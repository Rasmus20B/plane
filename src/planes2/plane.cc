#include "plane.h"
#include "projectile.h"

#include <random>

namespace plane {

  float rand_f(float max) {
    return static_cast<float>(rand() / static_cast<float>(static_cast<float>(RAND_MAX)/max));
  }

  void update_test_patterns(ProjectileSpace& transform) {
    auto vec = transform.position.norm();
    transform.position.vec.x += cos(RAD(transform.angle)) * transform.velocity.vec.x;
    transform.position.vec.y +=  sin(RAD(transform.angle)) * transform.velocity.vec.y;
    transform.angle+=1.5;
  }

  void main_loop() {
    srand(time(0));
    InitWindow(config.screen_width, config.screen_height, "Plane");
    SetTargetFPS(60);
    ProjectilePool e_ps;
    projectilePoolInit(e_ps, 5000);
    ProjectilePool p_ps;
    projectilePoolInit(p_ps, 250);
    EnemyPool enemies;
    enemyPoolInit(enemies, 40);
    std::unordered_map<int, Enemy> live_enemies;
    // Texture2D background = LoadTexture("../assets/city.png");
    float scroll = 0.0f;
    
    bool micro = false;
    Player p;

    load_stage1enemies(enemies);

    ProjectilePool test_patterns;
    projectilePoolInit(test_patterns, 200);

    Vec2 centre = Vec2(config.screen_width/2, config.screen_height/2);
    int n = 6;
    for(int j = 0; j < 3; ++j) {
    for(int i = 0; i < n; ++i) {
      addProjectile(test_patterns, ( std::move(Projectile{
        .position = Vec2(centre.vec.x + (15 * cos((360.0f/n) * i)), centre.vec.y + (15 * sin((360.0f/n) * i)) ),
        .old_position = Vec2(config.screen_width/2, config.screen_height/2),
        .velocity = { 4, 4 },
        .angle = (360.f / n) * i,
        .radius = 10,
        .spawntime = static_cast<float>(j),
        .live = true,
        })));
    }
    }
    while(!WindowShouldClose()) {

      float time = GetTime();
      // Controls
      //
      if(!p.d_time) {
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

      if(IsKeyDown(KEY_LEFT_SHIFT)) {
        micro = true;
        p.speed = 5.0f;
      } else if(IsKeyReleased(KEY_LEFT_SHIFT)) {
        micro = false;
        p.speed = 8.0f;
      }

      if(IsKeyDown(KEY_SPACE)) {
        if(time - p.last_shot > 0.10) {
          Projectile tmp = {
              .position = p.pos,
              .old_position = p.pos,
              .velocity = {0, 20},
              .radius = 10.0f,
              .attr = {},
              .live = true
          };
          addProjectile(p_ps, std::move(tmp));
          p.last_shot = time;
        }
      }
      }
      scroll += 1.0f;
      // if(scroll >= background.height*2) scroll = background.height;



      BeginDrawing();
        // DrawTextureEx(background, {0, scroll}, 0.0f, 2.0f, WHITE);
        // DrawTextureEx(background, { 0, background.height * 2 + scroll}, 0.0f, 2.0f, WHITE);
        // Handle enemies
        for(int i = 0; i < enemies.space.size() ; ++i) {
          if(enemies.spawntime[i] <= time && enemies.health[i] > 0) {
            DrawCircleV(enemies.space[i].position.vec, enemies.space[i].size, enemies.colours[i]);
            if(enemies.space[i].current_t >= enemies.space[i].points.size() - 1 ) {
              if(enemies.looped[i]) {
                enemies.space[i].current_t = 0;
              } else {
                goto e_shooting;
              }
            }
            enemies.space[i].position = enemies.space[i].points[enemies.space[i].current_t++];
e_shooting:
            if(!enemies.last_shots[i]) {
              addProjectile(e_ps, std::move(Projectile{
                .position = enemies.space[i].position,
                .old_position = enemies.space[i].position,
                .velocity = {0, 7.0f},
                .radius = 10.0f,
                .sprite = LoadTexture("../assets/orb2.png"),
                .live = true,
              }));
              enemies.last_shots[i] = 20;
            } else {
              enemies.last_shots[i]--;
            }
          }
        }

        for(int i = 0; i < e_ps.spaces.size(); ++i) {
          // For now just draw them increasing until they fall off the screen
          auto tmp = e_ps.spaces[i].old_position = e_ps.spaces[i].position;
          e_ps.spaces[i].position.vec = { tmp.vec.x, tmp.vec.y + e_ps.spaces[i].velocity.vec.y};
          if(!p.d_time && CheckCollisionRecs(Rectangle {
                p.pos.x, p.pos.y, 
                static_cast<float>(p.in_sprite.width), 
                static_cast<float>(p.in_sprite.height) }, 
                Rectangle{e_ps.spaces[i].position.vec.x, e_ps.spaces[i].position.vec.y, 
                static_cast<float>(e_ps.sprite[i].width), 
                static_cast<float>(e_ps.sprite[i].height)}
                )) {
            p.lives--;
            p.d_time = 50;
            micro = false;
            p.pos = {config.screen_width/2,(config.screen_height/8) * 6};
          }
          DrawTextureEx(e_ps.sprite[i], e_ps.spaces[i].position.vec, 0.0f, 1.0f, WHITE);
        }

        for(int i = 0; i < p_ps.spaces.size(); ++i) {
          // For now just draw them decreasing until they fall off the screen
          if(p_ps.live[i]) {
            auto p = p_ps.spaces[i].old_position = p_ps.spaces[i].position;
            for(int j = 0; j < enemies.space.size(); ++j) {
              if(enemies.health[j] && CheckCollisionCircles(p_ps.spaces[i].position.vec, p_ps.spaces[i].radius, enemies.space[j].position.vec, enemies.space[j].size)) {
                enemies.health[j]--;
                p_ps.live[i] = false;
              }
            }
            p_ps.spaces[i].position.vec = { p.vec.x, p.vec.y - p_ps.spaces[i].velocity.vec.y};
            DrawTextureEx(p_ps.sprite[i], p_ps.spaces[i].position.vec, 0.0f, 1.0f, WHITE);
          }
        }

        for(int i = 0; i < test_patterns.spaces.size(); ++i) {
          if(test_patterns.spawntime[i] > time) continue;
          update_test_patterns(test_patterns.spaces[i]);
          DrawCircleV(test_patterns.spaces[i].position.vec, test_patterns.spaces[i].radius, WHITE);
        }
        // Handle player
        if(!p.d_time) {
          DrawTextureEx(p.sprite, {p.pos.x - 16 * 3, p.pos.y - 16 *  3}, 0, 3.0f, WHITE);
          if(micro) DrawTextureEx(p.in_sprite, {p.pos.x - 2, p.pos.y - 2}, 0, 1, WHITE);
          // DrawCircleV(p.pos, p.size, RED);
          // DrawCircleV(p.pos, p.b_size, Color{255, 0, 0, 40});
        } else {
          if(!p.lives) return;
          p.d_time--;
        }

        DrawFPS(config.screen_width / 10, config.screen_height / 20);
        ClearBackground(GetColor(0x052c46ff));
      EndDrawing();
    }
  }
}
