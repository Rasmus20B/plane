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
};

}
