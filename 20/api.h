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

#include <bits/c++config.h>
#include <cstddef>
#include <iostream>
#include <map>
#include <vector>

enum class Orientation {
  Original000,
  Original090,
  Original180,
  Original270,
  Flipped000,
  Flipped090,
  Flipped180,
  Flipped270
};

enum class EdgeDirection {
  TopToRight,
  RightToDown,
  BottomToLeft,
  LeftToTop,
  TopToLeft,
  RightToTop,
  BottomToRight,
  LeftToBottom
};

inline EdgeDirection flip(EdgeDirection dir) {
  if (dir == EdgeDirection::TopToRight)
    return EdgeDirection::TopToLeft;
  if (dir == EdgeDirection::RightToDown)
    return EdgeDirection::RightToTop;
  if (dir == EdgeDirection::BottomToLeft)
    return EdgeDirection::BottomToRight;
  if (dir == EdgeDirection::LeftToTop)
    return EdgeDirection::LeftToBottom;
  if (dir == EdgeDirection::TopToLeft)
    return EdgeDirection::TopToRight;
  if (dir == EdgeDirection::RightToTop)
    return EdgeDirection::RightToDown;
  if (dir == EdgeDirection::BottomToRight)
    return EdgeDirection::BottomToLeft;
  if (dir == EdgeDirection::LeftToBottom)
    return EdgeDirection::LeftToTop;

  std::cerr << "PANIC\n";
  return EdgeDirection::LeftToTop;
}

inline int smaller(int a, int b) {
  if (a < b)
    return a;
  return b;
}

constexpr std::size_t BoarderLength(std::size_t i) { return i + 2; }
constexpr std::size_t TileSize(std::size_t i) {
  return BoarderLength(i) * BoarderLength(i);
}

template <std::size_t ImageLength> class TileTemplate {
public:
  constexpr static std::size_t Length = ImageLength;
  class EdgeProxy {
  private:
    EdgeProxy(const TileTemplate<ImageLength> &t, EdgeDirection d)
        : tile(t), dir(d) {}
    TileTemplate<ImageLength> const &tile;
    EdgeDirection dir;
    friend TileTemplate;

  public:
    class EdgeProxyPointIterator {
    private:
      TileTemplate<ImageLength> const &tile;
      EdgeDirection dir;
      std::size_t i;
      EdgeProxyPointIterator(TileTemplate<ImageLength> const &t,
                             EdgeDirection const d, std::size_t const j)
          : tile{t}, dir{d}, i{j} {}

    public:
      friend EdgeProxy;
      int operator*() {
        const std::size_t N = BoarderLength(ImageLength);
        if (dir == EdgeDirection::TopToRight)
          return tile.Raw[i];
        if (dir == EdgeDirection::RightToDown)
          return tile.Raw[i * N + (N - 1)];
        if (dir == EdgeDirection::BottomToLeft)
          return tile.Raw[N * N - 1 - i];
        if (dir == EdgeDirection::LeftToTop)
          return tile.Raw[N * (N - i - 1)];
        if (dir == EdgeDirection::TopToLeft)
          return tile.Raw[N - i - 1];
        if (dir == EdgeDirection::RightToTop)
          return tile.Raw[(N - i - 1) * N + (N - 1)];
        if (dir == EdgeDirection::BottomToRight)
          return tile.Raw[(N - 1) * N + i];
        if (dir == EdgeDirection::LeftToBottom)
          return tile.Raw[N * i];
      }
      EdgeProxyPointIterator &operator++() {
        ++i;
        return *this;
      }
      EdgeProxyPointIterator operator++(int) {
        EdgeProxyPointIterator retval(&tile, dir, i);
        ++i;
        return retval;
      }
      EdgeProxyPointIterator operator--(int) {
        EdgeProxyPointIterator retval(&tile, dir, i);
        --i;
        return retval;
      }
      EdgeProxyPointIterator &operator--() {
        --i;
        return *this;
      }
      friend bool operator==(const EdgeProxyPointIterator &lhs,
                             const EdgeProxyPointIterator &rhs) {
        // assert(&lhs.tile == &rhs.tile);
        // assert(lhs.dir == rhs.dir);
        return lhs.i == rhs.i;
      }
      friend bool operator!=(const EdgeProxyPointIterator &lhs,
                             const EdgeProxyPointIterator &rhs) {
        return !(lhs == rhs);
      }
    };
    EdgeProxyPointIterator begin() const {
      return EdgeProxyPointIterator{tile, dir, 0};
    }
    EdgeProxyPointIterator end() const {
      return EdgeProxyPointIterator{tile, dir, BoarderLength(ImageLength)};
    }
  };
  TileTemplate<ImageLength> () {}
  TileTemplate<ImageLength> (const TileTemplate<ImageLength>& rhs) : Id{rhs.Id} {
    for (std::size_t i = 0 ; i < TileSize(ImageLength) ; ++i) {
      Raw[i] = rhs.Raw[i];
    }
  }
  EdgeProxy edge(EdgeDirection dir) const { return EdgeProxy{*this, dir}; }

  int id() const { return Id; }

public:
  // private:
  int Id;
  int Raw[TileSize(ImageLength)];
};

template <int ImageLength>
int ComputFingerprint(
    typename TileTemplate<ImageLength>::EdgeProxy const &edge) {
  int acc = 0;
  int it = 0;
  for (auto v : edge) {
    acc += (1 << it++) * v;
  }
  return acc;
}

class Fingerprint {
  int Top;
  int Right;
  int Bottom;
  int Left;

  std::array<int, 4> Lowered;

public:
  // template <std::size_t ImageLength>
  // Fingerprint(const TileTemplate<ImageLength> &t)
  //     : Top{ComputFingerprint(t.edge(EdgeDirection::TopToRight))},
  //       Right{ComputFingerprint(t.edge(EdgeDirection::RightToDown))},
  //       Bottom{ComputFingerprint(t.edge(EdgeDirection::BottomToLeft))},
  //       Left{ComputFingerprint(t.edge(EdgeDirection::LeftToTop))} {
  //   int l = 0;
  //   for (EdgeDirection dir :
  //        {EdgeDirection::TopToRight, EdgeDirection::RightToDown,
  //         EdgeDirection::BottomToLeft, EdgeDirection::LeftToTop}) {
  //     Lowered[l++] = smaller(ComputFingerprint(t.edge(dir)),
  //                            ComputFingerprint(t.edge(flip(dir))));
  //   }
  // }
  Fingerprint() = default;
  Fingerprint(const Fingerprint&) = default;
  Fingerprint(Fingerprint&&) = default;
  Fingerprint& operator=(const Fingerprint&) = default;
  Fingerprint& operator=(Fingerprint&&) = default;
  Fingerprint(const TileTemplate<8> &t)
      : Top{ComputFingerprint<8>(t.edge(EdgeDirection::TopToRight))},
        Right{ComputFingerprint<8>(t.edge(EdgeDirection::RightToDown))},
        Bottom{ComputFingerprint<8>(t.edge(EdgeDirection::BottomToLeft))},
        Left{ComputFingerprint<8>(t.edge(EdgeDirection::LeftToTop))} {
    int l = 0;
    for (EdgeDirection dir :
         {EdgeDirection::TopToRight, EdgeDirection::RightToDown,
          EdgeDirection::BottomToLeft, EdgeDirection::LeftToTop}) {
      Lowered[l++] = smaller(ComputFingerprint<8>(t.edge(dir)),
                             ComputFingerprint<8>(t.edge(flip(dir))));
    }
  }
  std::array<int, 4> &lowered() { return Lowered; }
};

// template <std::size_t ImageLength>
// std::map<int, std::vector<TileTemplate<ImageLength> const *>>
// lut(std::vector<TileTemplate<ImageLength>> input) {
//   std::map<int, TileTemplate<ImageLength> const &> retval;
//   for (const auto &t : input) {
//     Fingerprint fp{t};
//     for (auto prints : fp.lowered()) {
//       retval[prints].push_back(&t);
//     }
//   }
// }
inline std::map<int, std::vector<TileTemplate<8>>>
lut(std::vector<TileTemplate<8>> input) {
  std::map<int, std::vector<TileTemplate<8>>> retval;
  for (const auto &t : input) {
    Fingerprint fp{t};
    for (auto prints : fp.lowered()) {
      retval[prints].push_back(t);
    }
  }
  return retval;
}
inline std::map<int, Fingerprint>
dofps (std::vector<TileTemplate<8>> input) {
  std::map<int, Fingerprint> retval;
  for (const auto &t : input) {
    Fingerprint fp{t};
    retval[t.Id] = fp;
  }
  return retval;
}

