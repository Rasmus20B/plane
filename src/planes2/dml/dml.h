#pragma once

#include <variant>
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <string_view>

namespace dml {

struct VM {
  std::array<char, 10192> memory;
  std::array<float, 20> vars;
  std::string pgtext{};
  uint32_t stackptr;
  uint32_t waitctr;
  uint32_t pc;
  
  void load_script(const std::string&& progtext);
  void init();
  void run();
  void stop();

  constexpr uint32_t getIntFromStack();
  constexpr uint32_t getIntFromArgument();
  constexpr void loadIntToStack(uint32_t num);
  constexpr void loadIntToStack();
};

enum class OpCodes  {
  NOP = 0,
  DELETE = 1,
  RETURN = 10,
  CALL = 11,
  JMP = 12,
  JUMPEQ = 13,
  JUMPNEQ = 14,
  CALLASYNC = 15,
  CALLASYNCID = 16,
  KILLASYNC = 17,
  KILLALLASYNC = 21,
  WAIT = 23,
  WAITF = 24,
  STACKALLOC = 40,
  PUSHI = 42,
  SETI = 43,
  PUSHF = 44,
  SETF = 45,
  ADDI = 50,
  ADDF = 51,
  SUBI = 52,
  SUBF = 53,
  MULI = 54,
  MULF = 55,
  DIVI = 56,
  DIVF = 57,
  MODI = 58,
  EQI = 59,
  EQF = 60,
  NEQI = 61,
  NEQF = 62,
  LESSI = 63,
  LESSF = 64,
  LEQI = 65,
  LEGF = 66,
  GREATERI = 67,
  GREATERF = 68,
  GEQI = 69,
  GEQF = 70,
  NOTI = 71,
  NOTF = 72,
  OR = 73,
  AND = 74,
  XOR = 75,
  BITOR = 76,
  BITAND = 77,
  DECI = 78,
  STACKSIN = 79,
  STACKCOS = 80,
  CIRCLEPOS = 81,
  VALIDRAD = 82,
  NEGI = 83,
  NEGF = 84,
  SQRSUM = 85,
  SQRSUMRT = 86,
  GETANGLE = 87,
  STACKSQRT = 88,
  LINEARFUNC = 89,
  POINTROTATE = 90,
  FLOATTIME = 91,

  ENMCREATE = 300,
  ENMCREATEA = 301,
  ANMSELECT = 302,

  MOVEPOS = 400,
  MOVPOSTIME = 401,

};

}
