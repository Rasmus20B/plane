#pragma once

#include <iterator>
#include <limits>
#include <vector>
#include "../enm.h"

struct Enm_Manager {

  Enm_Manager() {
    this->enms.resize(128);
    for(auto e : this->enms) {
      e.reset();
    }
  }

  std::optional<plane::Enm>& operator[](size_t pos) {
    return enms[pos];
  }

  uint16_t add_enm(plane::Enm&& enemy) {
    auto n = std::find_if(enms.begin(), enms.end(), [](auto e) {
        return (!e.has_value());
        });

    if(n != enms.end()) {
      n->emplace(std::move(enemy));
      return std::distance(enms.begin(), n);
    }
    return std::numeric_limits<uint16_t>().max();
  }

  std::vector<std::optional<plane::Enm>> enms;
};
