/*
 * Copyright (C) 2020  Paul Seyfert
 * Author: Paul Seyfert <pseyfert.mathphys@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
