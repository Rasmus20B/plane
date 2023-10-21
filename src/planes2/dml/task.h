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
    this->vars.resize(16);
    this->bm.resize(16);
    this->live_bms.resize(16);
    this->pc.resize(1);
    this->sp.resize(1);
    this->waitctr.resize(1);
    this->live_cos.resize(1);
  };

  std::vector<uint8_t> mem{};
  std::vector<float> vars{};
  std::vector<plane::BulletMgr> bm;
  std::vector<bool> live_bms;
  plane::Enm* e = nullptr;
  uint16_t cur_co = 0;
  std::vector<uint16_t> pc{};
  std::vector<uint16_t> sp{};
  std::vector<uint16_t> waitctr{};
  std::vector<bool> live_cos;
  bool base = 0;
  std::vector<uint16_t> children;

  void set_entry(uint16_t ep);
  void add_coroutine(uint16_t ep);
};
}
