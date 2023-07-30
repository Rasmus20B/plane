#include "dml.h"
#include <cstring>

namespace dml {

  void VM::load_script(const std::string&& progtext) {
    // Load text into the end of memory
    pgtext = std::move(progtext);
  }

  constexpr uint32_t VM::getIntFromStack(){
    uint32_t num = (memory[stackptr-4] << 24) | (memory[stackptr-3] << 16) | 
      (memory[stackptr-2] << 8) | memory[stackptr-1];
    return num;
  }

  constexpr uint32_t VM::getIntFromArgument() {
    uint32_t num;
    num |= this->pgtext[pc+1];
    num |= this->pgtext[pc+2];
    num |= this->pgtext[pc+3];
    num |= this->pgtext[pc+4];
    return num;
  }

  void VM::init() {
    this->pc = 0;
    this->waitctr = 0;
    this->stackptr = 0;
  }

  constexpr void VM::loadIntToStack() {
    this->memory[stackptr++] = this->pgtext[pc+1];
    this->memory[stackptr++] = this->pgtext[pc+2];
    this->memory[stackptr++] = this->pgtext[pc+3];
    this->memory[stackptr++] = this->pgtext[pc+4];
  }

  void VM::run() {

    while(true) {
      if(this->waitctr) {
        this->waitctr--;
        return;
      }
      int opcode = pgtext[pc];
      opcode = opcode << 16;
      this->pc++;
      opcode |= pgtext[pc];
      OpCodes oc = static_cast<OpCodes>(opcode);
      switch(oc) {
        case OpCodes::NOP:
          // nop
          pc++;
        case OpCodes::DELETE:
          // delete execution
          pc++;
          break;
        case OpCodes::JMP:
          // jmp
          pc++;
          break;
        case OpCodes::JUMPEQ:
          pc++;
          break;
        case OpCodes::WAIT: {
          // wait
          uint32_t time = 0;
          time |= (pgtext[pc+4]);
          this->waitctr = time;
          pc += 5;
          break;
          };
          break;
        case OpCodes::RETURN:
          // Return
          break;
        case OpCodes::PUSHI:
          // push i
          {
          loadIntToStack();
          this->pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::SETI:
          // set i
          {
          uint32_t var = (pgtext[pc+1] << 24) | (pgtext[pc+2] << 16) | (pgtext[pc+3] << 8) | pgtext[pc+4];
          uint32_t num = (memory[stackptr-4] << 24) | (memory[stackptr-3] << 16) | (memory[stackptr-2] << 8) | memory[stackptr-1];
          std::cout << "OVERWRITING VAR: " << var << " WITH " << num << "\n";
          this->vars[var] = num;
          this->pc += sizeof(int) + 1;
          this->stackptr -= 5;
          break;
          }
        case OpCodes::PUSHF:
          // push f
          {
          int num = (pgtext[pc+1] << 24) | (pgtext[pc+2] << 16) | (pgtext[pc+3] << 8) | pgtext[pc+4];
          loadIntToStack();
          this->pc += sizeof(int) + 1;
          break;
          }
        case OpCodes::SETF:
          // set f
          {
          uint32_t var = (pgtext[pc+1] << 24) | (pgtext[pc+2] << 16) | (pgtext[pc+3] << 8) | pgtext[pc+4];
          uint32_t num = (memory[stackptr-4] << 24) | (memory[stackptr-3] << 16) | (memory[stackptr-2] << 8) | memory[stackptr-1];
          this->vars[var] = num;
          this->pc += sizeof(int) + 1;
          this->stackptr -= 5;
          break;
          }
        case OpCodes::ADDI:
          {
          // add i 
          uint32_t res = 0;
          uint32_t o1 = getIntFromStack();
          this->stackptr -= 4;
          uint32_t o2 = getIntFromStack();
          this->stackptr -= 4;
          res = o1 + o2;

          this->memory[this->stackptr++] = res << 24;
          this->memory[this->stackptr++] = res << 16;
          this->memory[this->stackptr++] = res << 8;
          this->memory[this->stackptr++] = res;
          this->pc += sizeof(int);
          break;
          }
        case OpCodes::ADDF: 
          {
          // add f 
          float res = 0;
          float o1 = getIntFromStack();
          this->stackptr -= 4;
          float o2 = getIntFromStack();
          this->stackptr -= 4;
          res = o1 + o2;

          std::cout << "GOT " << res << "\n";

          this->memory[this->stackptr++] = *(uint32_t*)&res << 24;
          this->memory[this->stackptr++] = *(uint32_t*)&res << 16;
          this->memory[this->stackptr++] = *(uint32_t*)&res << 8;
          this->memory[this->stackptr++] = *(uint32_t*)&res ;
          this->pc += sizeof(int);
          break;
          }


        case OpCodes::ENMCREATE:
          // enmCreate()
          break;

        case OpCodes::MOVEPOS:
          // movPos(x, y)
          break;

        case OpCodes::MOVPOSTIME:
          // movPosTime(x, y, t)
          break;
        default:
          std::cout << "OPCODE NOT IMPLEMENTED\n";
          return;
      }
    }
  }

  void VM::stop() {
    
  }
}

