#include "dml.h"

#include "../input.h"
#include "scheduler.h"
#include <chrono>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iterator>
#include <ratio>
#include <raylib.h>
#include <thread>

namespace dml {

  bool VM::addBM(plane::BulletMgr bm) {
    uint16_t i = 0;
    while(i < this->bullets_mask.size()) {
      if(this->bullets_mask[i] == false) {
        bm.id = i;
        bm.setOrigin(CURTASK.e.spatial.abspos + CURTASK.e.spatial.relpos);
        bm.shoot(bm.origin, p.spatial.pos);
        this->bullets[i] = bm;
        this->bullets_mask[i] = true;
        return true;
      }
      i++;
    }
    return false;
  }

  void VM::removeBM(const uint16_t id) {
    this->bullets_mask[id] = false;
  }

  void VM::load_script(const std::vector<uint8_t>&& progtext) {
    pgtext = std::move(progtext);
  }

  [[nodiscard]]
  uint32_t VM::popInt(const uint32_t t_id){
    uint32_t num = 0;
    assert(CURTASK.sp > 0 && "Stack should not be empty before popping");
    CURTASK.sp -= 4;
    num += (CURTASK.mem[CURTASK.sp+3] << 24) & 0xFF000000;
    num += (CURTASK.mem[CURTASK.sp+2] << 16) & 0x00FF0000;
    num += (CURTASK.mem[CURTASK.sp+1] << 8) & 0x0000FF00;
    num += (CURTASK.mem[CURTASK.sp] & 0x000000FF);
    return num;
  }

  [[nodiscard]]
  uint32_t VM::getIntFromArgument(const uint32_t t_id) {
    uint32_t num = 0;
    num += this->pgtext[CURTASK.pc+1] & 0x000000FF;
    num += (this->pgtext[CURTASK.pc+2] << 8) & 0x0000FF00;
    num += (this->pgtext[CURTASK.pc+3] << 16) & 0x00FF0000;
    num += (this->pgtext[CURTASK.pc+4] << 24) & 0xFF000000;
    return num;
  }
  
  void VM::init(const uint32_t t_id, const uint32_t start) {
    this->sch.tasks[t_id].pc = start;
    this->sch.tasks[t_id].waitctr = 0;
    this->sch.tasks[t_id].sp = 0;
    std::fill(this->bullets_mask.begin(), this->bullets_mask.end(), false);
    std::fill(this->sch.tasks[t_id].mem.begin(), this->sch.tasks[t_id].mem.end(), 0);
  }

  void VM::pushInt(const uint32_t t_id, const uint32_t num) {
    CURTASK.mem[CURTASK.sp] = num & 0x000000FF;
    CURTASK.mem[CURTASK.sp+1] = (num & 0x0000FF00) >> 8;
    CURTASK.mem[CURTASK.sp+2] = (num & 0x00FF0000) >> 16;
    CURTASK.mem[CURTASK.sp+3] = (num & 0xFF000000) >> 24;
    CURTASK.sp += 4;
  }

  void VM::pushInt(const uint32_t t_id) {
    CURTASK.mem[CURTASK.sp] = this->pgtext[CURTASK.pc+1];
    CURTASK.mem[CURTASK.sp + 1] = this->pgtext[CURTASK.pc+2];
    CURTASK.mem[CURTASK.sp + 2] = this->pgtext[CURTASK.pc+3];
    CURTASK.mem[CURTASK.sp + 3] = this->pgtext[CURTASK.pc+4];
    CURTASK.sp += 4;
  }

  void VM::handle_bullets() {
    for(uint32_t id = 0; id < this->bullets_mask.size() - 1; ++id) {
      if(this->bullets_mask[id] == true) {
        this->bullets[id].update();
        this->bullets[id].draw();

        if(this->bullets[id].collision_check(
              {
              .x = p.spatial.pos.x,
              .y = p.spatial.pos.y,
              .width = (float)p.sprite.width,
              .height = (float)p.sprite.height
              })) {
        }

        this->bullets[id].setOutOfBounds();

        bool ded = (std::adjacent_find(bullets[id].oobs.begin(), 
              bullets[id].oobs.end(), 
              std::not_equal_to<bool>()) == bullets[id].oobs.end());
        if(ded) {
          removeBM(id);
        }
      }
    }
  }

  void VM::fetch_execute() {
    return;
  }
  
  void VM::read_header() {
    constexpr std::array magic = { 0x7f, 0x44, 0x4d, 0x4c };
    for(int i = 0; i < 4; ++i) {
      if(magic[i] != pgtext[i]) {
        std::cerr << "unknown file type\n";
        break;
      }
    }

    // Set pc to the entry point found in header file
    this->sch.tasks[0].pc = this->pgtext[4] + this->pgtext[5];
    pgtext.erase(pgtext.begin(), pgtext.begin() + 5);
  }

  void VM::run() {

    double time_acc = 0.f;

    sch.set_ts_duration(16.67);
    sch.init();
    this->init(0, 0);

    read_header();

    while(!WindowShouldClose()) {
      ClearBackground(BLACK);

      if(this->power.test()) {
        return;
      }


      if(sch.c_task == 0) {
        plane::handle_game_input(p.spatial,  p.shooting);
        BeginDrawing();
        for(uint32_t i = 0; i < sch.tasks_mask.size(); ++i){
          if(sch.tasks_mask[i] != true) continue;

          // Update Enemy positionsS
          if(!sch.tasks[i].waitctr) {
            sch.tasks[i].e.spatial.abspos.vec.x += std::round(sch.tasks[i].e.spatial.absspeed * cos(RAD(sch.tasks[i].e.spatial.absang)));
            sch.tasks[i].e.spatial.abspos.vec.y += std::round(sch.tasks[i].e.spatial.absspeed * sin(RAD(sch.tasks[i].e.spatial.absang)));
            sch.tasks[i].e.spatial.relpos.vec.x += std::round(sch.tasks[i].e.spatial.relspeed * cos(RAD(sch.tasks[i].e.spatial.relang)));
            sch.tasks[i].e.spatial.relpos.vec.y += std::round(sch.tasks[i].e.spatial.relspeed * sin(RAD(sch.tasks[i].e.spatial.relang)));
            sch.tasks[i].e.spatial.time--;
          }


          // Draw Enemies
          DrawTextureV(CURTASK.e.sprite, {
              (CURTASK.e.spatial.abspos.x() + CURTASK.e.spatial.relpos.x()) - (CURTASK.e.sprite.width * 0.5f), 
              (CURTASK.e.spatial.abspos.y() + CURTASK.e.spatial.relpos.y()) - (CURTASK.e.sprite.height * 0.5f)
              }, CURTASK.e.col);
        }

        // handle bullets
        handle_bullets();
        DrawFPS(p.spatial.pos.x, p.spatial.pos.y);
        EndDrawing();
      }

      if(time_acc >= sch.cur_slice) {
        time_acc = 0.f;
        if(!sch.next_task()) return;
      }

      if(CURTASK.waitctr && CURTASK.e.spatial.time) {
        CURTASK.waitctr -= 1;
        if(!sch.next_task()) return;
        continue;
      }

      /* If we reach the end of our sub, then delete the thread */
      if(CURTASK.pc >= this->pgtext.size() ) {
        std::cout << "EOF\n";
        sch.del_task();
        if(!sch.next_task()) {
          return;
        }
        continue;
      }

      auto start_time = std::chrono::high_resolution_clock::now();
      uint32_t opcode = pgtext[CURTASK.pc];
      opcode = opcode << 8;
      CURTASK.pc++;
      opcode |= pgtext[CURTASK.pc];
      std::cout << "THREAD " << std::dec << sch.c_task << " @" << CURTASK.pc << ": " << std::hex << opcode << "\n";
      OpCodes oc = static_cast<OpCodes>(opcode);
      switch(oc) {
        case OpCodes::NOP:
          CURTASK.pc++;
          break;
        case OpCodes::DELETE:
          // delete execution
          sch.del_task();
          if(sch.n_tasks == 0) return;
          break;
        case OpCodes::RETURN:
          CURTASK.pc = popInt(sch.c_task) ;
          break;
        case OpCodes::CALL:
          pushInt(sch.c_task, CURTASK.pc + sizeof(int) + 1);
          CURTASK.pc = getIntFromArgument(sch.c_task);
          break;
        case OpCodes::JMP:
          // jmp
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          CURTASK.pc = addr ;
          break;
          }
        case OpCodes::JMPEQ: 
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          uint32_t test = popInt(sch.c_task);
          if(test == 0) {
            CURTASK.pc = addr ;
          } else {
            CURTASK.pc+=sizeof(int) + 1;
          }
          break;
          }
        case OpCodes::JMPNEQ:
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          uint32_t test = popInt(sch.c_task);
          std::cout << std::dec << test << "\n";
          if(test > 0) {
            CURTASK.pc = addr;
          } else {
            CURTASK.pc += sizeof(int) + 1;
          }
          break;
          }
        case OpCodes::WAIT: {
          // wait
          uint32_t time = getIntFromArgument(sch.c_task);
          CURTASK.waitctr = time;
          CURTASK.pc += sizeof(int) + 1;
          break;
          };
        case OpCodes::PUSHI:
          // push i
          {
          uint32_t var = getIntFromArgument(sch.c_task);
          pushInt(sch.c_task, var);
          CURTASK.pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::SETI:
          // set i
          {
          uint32_t var = getIntFromArgument(sch.c_task);
          uint32_t num = popInt(sch.c_task);
          std::cout << "setting #" << var << " to " << num << "\n";
          CURTASK.vars[var] = num;
          CURTASK.pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::PUSHF:
          // push f
          {
          break;
          }
        case OpCodes::SETF:
          // set f
          {
          }
        case OpCodes::ADDI:
          {
          // add i 
          uint32_t res = 0;
          while(CURTASK.sp > 0) {
            res += popInt(sch.c_task);
          }

          pushInt(sch.c_task, res);
          CURTASK.pc++;
          break;
          }
        case OpCodes::ADDF: 
          // add f 
          {
          break;
          }

        /* ENEMY CREATION AND ANM SCRIPT MANAGEMENT */
        case OpCodes::ENMCREATE:
          // enmCreate()
          {
          uint32_t addr = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          if(!sch.add_task(addr)) {
            std::cout << "Unable to add new task\n";
          }
          break;
          }
        case OpCodes::ENMCREATEA:
          {
          CURTASK.pc+=sizeof(int) + 1;
          break;
          }
        case OpCodes::ANMSELECT:
          {
          uint32_t spr = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::ANMSETSPRITE:
          {
          uint32_t spr = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t scr = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.e.sprite = plane::tm.actSprites[spr];
          CURTASK.e.col = RAYWHITE;
          break;
          }
        case OpCodes::ANMSETMAIN:
          {
          uint32_t slot = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t scr = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::ENMDELETE:
          sch.del_task();
          if(!sch.next_task()) return;
          break;

        case OpCodes::MOVEPOS:
          // movPos(x, y)
          { 
          uint32_t x = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t y = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.e.spatial.abspos = {(float)x , (float)y};
          break;
          }
        case OpCodes::MOVEPOSTIME:
          // movPosTime(x, y, t)
          CURTASK.pc += sizeof(int) * 3 + 1;
          break;

        case OpCodes::MOVEPOSRELTIME:
          break;

        case OpCodes::MOVEVEL:
          {
          uint32_t ang = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int));
          uint32_t spd = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int)) + 1;
          CURTASK.e.spatial.absang = ang;
          CURTASK.e.spatial.absspeed = spd;
          break;
          }
        case OpCodes::MOVEVELTIME:
          {
          uint32_t time = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int));
          uint32_t mode = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int));
          float ang = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int));
          float spd = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int)) + 1;
          CURTASK.e.spatial.absang = ang;
          CURTASK.e.spatial.absspeed = spd;
          CURTASK.e.spatial.time = time;
          CURTASK.e.spatial.movement = static_cast<plane::enmMoveType>(mode);
          break;
          }
        case OpCodes::MOVEVELREL:
          {
          uint32_t ang = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int));
          uint32_t spd = getIntFromArgument(sch.c_task);
          CURTASK.pc += (sizeof(int)) + 1;
          CURTASK.e.spatial.relang = ang;
          CURTASK.e.spatial.relspeed = spd;
          break;
          }

        /* Bullet Management */
        case OpCodes::ETNEW:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          plane::BulletMgr b {
            .mode = plane::BulletFlag::AIMED,
          };
          b.setType(plane::BulletSprite::BLADE_01);
          CURTASK.bm[idx] = b;
          CURTASK.pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::ETON:
          {
            std::cout << "SHOOTING BULLET\n";
          uint32_t idx = getIntFromArgument(sch.c_task);
          addBM(CURTASK.bm[idx]);
          CURTASK.pc+= sizeof(int) + 1;
          break;
          }
        case OpCodes::ETSPRITE:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t sprite = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t colour = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.bm[idx].setType(static_cast<plane::BulletSprite>(sprite));
          break;
          }
        case OpCodes::ETOFFSET:
          break;
        case OpCodes::ETANGLE:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t ang1 = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t ang2 = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.bm[idx].setAngle(ang1, ang2);
          break;
          }
        case OpCodes::ETSPEED:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t sp1 = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t sp2 = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.bm[idx].setSpeed(sp1, sp2);
          break;
          }
        case OpCodes::ETCOUNT:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t la = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t co = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.bm[idx].setCount(la, co);
          break;
          }
        case OpCodes::ETAIM:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) ;
          uint32_t aim = getIntFromArgument(sch.c_task);
          CURTASK.pc += sizeof(int) + 1 ;
          CURTASK.bm[idx].setAim(static_cast<plane::BulletFlag>(aim));
          break;
          }
        default:
          std::cout << "OPCODE NOT IMPLEMENTED: " << opcode << "\n";
          return;
      }
      auto dur_taken = std::chrono::high_resolution_clock::now();
      std::chrono::duration<float, std::milli> acc = dur_taken - start_time;
      time_acc += acc.count();
    }
  }

  void VM::stop() {
    this->power.test_and_set();
  }
}

