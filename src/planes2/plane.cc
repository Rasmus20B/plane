#include "plane.h"
#include "projectile.h"

#include <random>
#include <raylib.h>

namespace plane {

  float rand_f(float max) {
    return static_cast<float>(rand() / static_cast<float>(static_cast<float>(RAND_MAX)/max));
  }

  void main_loop() {
    srand(time(0));
    InitWindow(config.screen_width, config.screen_height, "Plane");
    SetTargetFPS(60);

    TextureManager tm;
    tm.textures.reserve(10);
    initTextureManager(tm);

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

    Vec2 centre = Vec2(config.screen_width/2, config.screen_height/2);
    int n = 5;
    for(int j = 0; j < 3; ++j) {
      for(int i = 0; i < n; ++i) {
        addProjectile(e_ps, ( std::move(Projectile{
          .position = Vec2(centre.vec.x + (15 * cos((360.0f/n) * i)), centre.vec.y + (15 * sin((360.0f/n) * i)) ),
          .old_position = Vec2(config.screen_width/2, config.screen_height/2),
          .velocity = { 4, 4 },
          .angle = (360.f / n) * i,
          .radius = 10,
          .sprite = tm.textures[4],
          .spawntime = static_cast<float>(j),
          .mt = MoveType::MOVE_CIRCLE,
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
              .sprite = tm.textures[1],
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
                .sprite = tm.textures[5],
                .mt = MoveType::MOVE_NORM,
                .live = true,
              }));
              enemies.last_shots[i] = 20;
            } else {
              enemies.last_shots[i]--;
            }
          }
        }

        for(int i = 0; i < e_ps.spaces.size(); ++i) {
          if(e_ps.spawntime[i] > time) continue;
          // For now just draw them increasing until they fall off the screen
          pMove(e_ps.spaces[i]);
          DrawTextureEx(e_ps.sprite[i], 
              { 
                e_ps.spaces[i].position.vec.x - (e_ps.sprite[i].width / 2.0f),
                e_ps.spaces[i].position.vec.y - (e_ps.sprite[i].height / 2.0f)
              },
              e_ps.spaces[i].angle, 1.0f, WHITE);
          if(!p.d_time && CheckCollisionRecs(Rectangle {
                p.pos.x - (p.sprite.width / 2.0f), p.pos.y - (p.sprite.height / 2.0f), 
                static_cast<float>(p.in_sprite.width), 
                static_cast<float>(p.in_sprite.height) }, 
                Rectangle{e_ps.spaces[i].position.vec.x - (e_ps.sprite[i].width / 2.0f), 
                e_ps.spaces[i].position.vec.y - (e_ps.sprite[i].height / 2.0f),
                static_cast<float>(e_ps.sprite[i].width ), 
                static_cast<float>(e_ps.sprite[i].height )}
                )) {
            p.lives--;
            p.d_time = 50;
            micro = false;
            p.pos = {config.screen_width/2,(config.screen_height/8) * 6};
          }


#ifdef DRAW_HITBOX
        DrawRectanglePro(Rectangle {
                e_ps.spaces[i].position.vec.x - (e_ps.sprite[i].width / 2.0f), e_ps.spaces[i].position.vec.y - (e_ps.sprite[i].height / 2.0f), 
                static_cast<float>(e_ps.sprite[i].width), 
                static_cast<float>(e_ps.sprite[i].height) }, 
                Vector2 { 0, 0 },
                (e_ps.spaces[i].angle), GetColor(0x00ff0073));
#endif
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
            DrawTextureV(p_ps.sprite[i], {
                p_ps.spaces[i].position.vec.x - (p_ps.sprite[i].width / 2.0f),
                p_ps.spaces[i].position.vec.y - (p_ps.sprite[i].height / 2.0f)
                }, WHITE);
            p_ps.spaces[i].position.vec = { p.vec.x, p.vec.y - p_ps.spaces[i].velocity.vec.y};
          }
        }

        // Handle player
        if(!p.d_time) {
          DrawTextureV(p.sprite, {p.pos.x - (p.sprite.width / 2.0f), p.pos.y - (p.sprite.height / 2.0f) }, WHITE);
          if(micro) DrawTextureV(p.in_sprite, {p.pos.x - (p.in_sprite.width / 2.0f)  , p.pos.y - (p.in_sprite.height / 2.0f) }, WHITE);
        } else {
          if(!p.lives) return;
          p.d_time--;
        }

#ifdef DRAW_HITBOX
        DrawRectangleRec(Rectangle {
                p.pos.x - (p.sprite.width / 2.0f), p.pos.y - (p.sprite.height / 2.0f), 
                static_cast<float>(p.sprite.width), 
                static_cast<float>(p.sprite.height) }, GetColor(0xff000073));
#endif

        DrawFPS(config.screen_width / 10, config.screen_height / 20);
        ClearBackground(GetColor(0x052c46ff));
      EndDrawing();
    }
  }
}
