#include "dml.h"

#include "../input.h"
#include <bits/chrono.h>
#include <chrono>
#include <cstring>
#include <iterator>
#include <ratio>
#include <raylib.h>
#include <thread>

namespace dml {

  

  void VM::load_script(const std::vector<uint8_t>&& progtext) {
    // Load text into the end of memory
    pgtext = std::move(progtext);
  }

  uint32_t VM::getIntFromStack(const uint32_t t_id){
    uint32_t num = 
        (CURTASK.mem[CURTASK.sp-4] << 24)
      | (CURTASK.mem[CURTASK.sp-3] << 16) 
      | (CURTASK.mem[CURTASK.sp-2] << 8) 
      |  CURTASK.mem[CURTASK.sp-1];
    CURTASK.sp -= 4;
    return num;
  }

  uint32_t VM::getIntFromArgument(const uint32_t t_id) {
    uint32_t num = 0;
    num += this->pgtext[CURTASK.pc+1] & 0x000000FF;
    num += (this->pgtext[CURTASK.pc+2] << 8) & 0x0000FF00;
    num += (this->pgtext[CURTASK.pc+3] << 16) & 0x00FF0000;
    num += (this->pgtext[CURTASK.pc+4] << 24) & 0xFF000000;
    return num;
  }
  
  void VM::init(const uint32_t t_id, const uint32_t start) {
    CURTASK.pc = start;
    CURTASK.waitctr = 0;
    CURTASK.sp = 0;
    std::fill(CURTASK.live_bms.begin(), CURTASK.live_bms.end(), false);
  }

  void VM::loadIntToStack(const uint32_t t_id, const uint32_t num) {
    CURTASK.mem[CURTASK.sp+1] = num << 24;
    CURTASK.mem[CURTASK.sp+2] = num << 16;
    CURTASK.mem[CURTASK.sp+3] = num << 8;
    CURTASK.mem[CURTASK.sp+4] = num;
  }

  void VM::loadIntToStack(const uint32_t t_id) {
    CURTASK.mem[CURTASK.sp+1] = this->pgtext[CURTASK.pc+1];
    CURTASK.mem[CURTASK.sp+2] = this->pgtext[CURTASK.pc+2];
    CURTASK.mem[CURTASK.sp+3] = this->pgtext[CURTASK.pc+3];
    CURTASK.mem[CURTASK.sp+4] = this->pgtext[CURTASK.pc+4];
  }

  void VM::fetch_execute() {
    return;
  }

  void VM::run() {

    float time_acc = 0.f;

    sch.set_ts_duration(16.67);
    sch.init();

    while(!WindowShouldClose()) {
      ClearBackground(BLACK);

      if(this->power.test()) {
        return;
      }

      plane::handle_game_input(p.spatial,  p.shooting);

      if(sch.c_task == 0) {
        BeginDrawing();
        for(uint32_t i = 0; i < sch.tasks_mask.size(); ++i){
          if(sch.tasks_mask[i] != true) continue;
          for(uint32_t b = 0; b < sch.tasks[i].bm.size(); ++b) {
            if(sch.tasks[i].live_bms[b] == true) {
              sch.tasks[i].bm[b].update();
              sch.tasks[i].bm[b].draw();
            }
          }
        }
        DrawFPS(p.spatial.pos.x, p.spatial.pos.y);
        EndDrawing();
      }

      if(time_acc >= sch.cur_slice) {
        time_acc = 0.f;
        if(!sch.next_task()) return;
      }


      if(CURTASK.waitctr) {
        CURTASK.waitctr -= 1;
        if(!sch.next_task()) {
          return;
        }
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
      int opcode = pgtext[CURTASK.pc];
      opcode = opcode << 8;
      CURTASK.pc++;
      opcode |= pgtext[CURTASK.pc];
      std::cout << "THREAD " << std::dec << sch.c_task << " @" << CURTASK.pc << ": " << std::hex << opcode << "\n";
      OpCodes oc = static_cast<OpCodes>(opcode);
      switch(oc) {
        case OpCodes::NOP:
          // nop
          CURTASK.pc++;
        case OpCodes::DELETE:
          // delete execution
          CURTASK.pc++;
          break;
        case OpCodes::JMP:
          // jmp
          {
          uint32_t n = getIntFromArgument(sch.c_task);
          CURTASK.pc = n;
          break;
          }
        case OpCodes::JUMPEQ:
          CURTASK.pc++;
          break;
        case OpCodes::WAIT: {
          // wait
          uint16_t time = getIntFromArgument(sch.c_task);
          std::cout << "waiting for " << std::dec << time << " frames\n";
          CURTASK.waitctr = time;
          CURTASK.pc += sizeof(int) + 1;
          break;
          };
        case OpCodes::RETURN:
          // Return
          break;
        case OpCodes::PUSHI:
          // push i
          {
          loadIntToStack(sch.c_task);
          CURTASK.pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::SETI:
          // set i
          {
          uint32_t var = getIntFromArgument(sch.c_task);
          uint32_t num = getIntFromStack(sch.c_task);
          CURTASK.vars[var] = num;
          CURTASK.pc += sizeof(int) + 1;
          CURTASK.sp -= 4;
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
          uint32_t o1 = getIntFromStack(sch.c_task);
          CURTASK.sp -= 4;
          uint32_t o2 = getIntFromStack(sch.c_task);
          CURTASK.sp -= 4;
          res = o1 + o2;

          loadIntToStack(sch.c_task, res);
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
          std::cout << "STARTING TASK @ " << std::hex << addr << "\n";
          CURTASK.pc += sizeof(int) + 1;
          if(!sch.add_task(addr)) {
            std::cout << "Unable to add new task\n";
          }
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
          std::cout << std::dec << "MOVING TO " << x << " , " << y << "\n";
          CURTASK.pos.vec = {(float)x , (float)y};
          break;
          }

        case OpCodes::MOVPOSTIME:
          // movPosTime(x, y, t)
          break;

        /* Bullet Management */
        case OpCodes::ETNEW:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          plane::BulletMgr b {
            .mode = plane::BulletFlag::AIMED,
          };
          b.setType(plane::BulletSprite::BLADE_01);
          CURTASK.bm[idx] = b;
          CURTASK.pc+= sizeof(int) + 1;
          break;
          }
        case OpCodes::ETON:
          {
          uint32_t idx = getIntFromArgument(sch.c_task);
          CURTASK.live_bms[idx] = true;
          CURTASK.bm[idx].shoot(CURTASK.pos, p.spatial.pos);
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
          std::cout << "OPCODE NOT IMPLEMENTED\n";
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

