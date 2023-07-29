#include "dml.h"
#include <cstring>

namespace dml {

  void VM::load_script(const std::string&& progtext) {
    // Load text into the end of memory
    pgtext = std::move(progtext);
  }

  void VM::init() {
    this->pc = 0;
    this->waitctr = 0;
    this->stackptr = 0;
  }

  void VM::run() {

    while(true) {
      if(this->waitctr) {
        this->waitctr--;
        return;
      }
      uint32_t opcode = pgtext[pc];
      opcode = opcode << 16;
      this->pc++;
      opcode |= (uint32_t)pgtext[pc];
      switch(opcode) {
        case 0x0000:
          // nop
          pc++;
        case 0x0001:
          // delete execution
          pc++;
          continue;
        case 0x0002:
          // jmp
          pc++;
          continue;
        case 0x0017: {
          // wait
          uint32_t time = 0;
          time |= (pgtext[pc+4]);
          this->waitctr = time;
          pc += 5;
          break;
          };
        case 0x000D: 
          continue;
        case 0x0004:
          break;
        case 0x000A:
          // Return
          break;
        case 0x002A:
          // push i
          {
          int num = (pgtext[pc+1] << 24) | (pgtext[pc+2] << 16) | (pgtext[pc+3] << 8) | pgtext[pc+4];
          this->memory[stackptr++] = this->pgtext[pc+1];
          this->memory[stackptr++] = this->pgtext[pc+2];
          this->memory[stackptr++] = this->pgtext[pc+3];
          this->memory[stackptr++] = this->pgtext[pc+4];
          this->pc += sizeof(int) + 1;
          continue;
          }
        case 0x002B:
          {
          uint32_t var = (pgtext[pc+1] << 24) | (pgtext[pc+2] << 16) | (pgtext[pc+3] << 8) | pgtext[pc+4];
          uint32_t num = (memory[stackptr-4] << 24) | (memory[stackptr-3] << 16) | (memory[stackptr-2] << 8) | memory[stackptr-1];
          this->vars[var] = num;
          this->pc += sizeof(int) + 1;
          this->stackptr -= 5;
          }
        case 0x0032:{
          uint32_t res = 0;
          while(stackptr > 0) {
            uint32_t o = (memory[stackptr-4] << 24) | (memory[stackptr-3] << 16) | (memory[stackptr-2] << 8) | memory[stackptr-1];
            res += o;
            this->stackptr -= 4;
          }
          pc += 3;
          };

        default:
          std::cout << "OPCODE NOT IMPLEMENTED\n";
          return;
      }
    }
  }

  void VM::stop() {
    
  }
}

