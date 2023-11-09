#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

#include "../../planes2/enm.h"
#include "coroutine.h"

namespace dml {


struct Task {

  Task() {
    this->mem.resize(200);
    this->execution_stack.resize(30);
    this->vars.resize(16);
    this->bm.resize(16);
    this->live_bms.resize(16);
  };

  std::vector<uint8_t> mem{};
  std::vector<uint32_t> execution_stack{};
  std::vector<float> vars{};
  std::vector<plane::BulletMgr> bm;
  std::vector<bool> live_bms;
  plane::Enm* e = nullptr;
  uint16_t pc{};
  uint16_t sp{};
  uint16_t waitctr{};
  bool base = 0;
  std::vector<uint16_t> children;

  void set_entry(uint16_t ep);
  void add_coroutine(uint16_t ep);
};
}
