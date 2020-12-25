/// would love if this worked, but had some issues with c++20
#include <bits/c++config.h>
#include <cstddef>
#include <iostream>
#include <map>
#include <span>
#include <vector>
#include "api.h"

template <std::size_t ImageLength>
std::map<int, std::vector<TileTemplate<ImageLength> const *>>
lut(std::span<TileTemplate<ImageLength>> input) {
  std::map<int, TileTemplate<ImageLength> const &> retval;
  for (const auto &t : input) {
    Fingerprint fp{t};
    for (auto prints : fp.lowered()) {
      retval[prints].push_back(&t);
    }
  }
}

// int main() {
//   std::vector<TileTemplate<8>> v;
//   auto lut(v);
//   return 0;
// }
