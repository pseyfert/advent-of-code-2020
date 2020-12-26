#include <boost/circular_buffer.hpp>
#include <boost/circular_buffer/base.hpp>
#include <compare>
#include <iostream>
#include <string>

struct cupvalue {
  int val;
  explicit cupvalue(int v) {
    if (v >= 0) {
      val = v % 10;
    } else {
      int add_at_least = -v;
      add_at_least -= v % 10;
      add_at_least += 10;
      if ((add_at_least % 10) != 0) {
        std::cout << "BIG mistake 1\n";
      }
      val = v + add_at_least;
      if (val < 0) {
        std::cout << "BIG mistake 2\n";
      }
    }
  }
  cupvalue() {}
  cupvalue &operator=(const cupvalue &) = default;
  cupvalue &operator=(cupvalue &&) = default;
  cupvalue(const cupvalue &) = default;
  cupvalue(cupvalue &&) = default;
  friend cupvalue operator-(const cupvalue &lhs, int rhs) {
    return cupvalue(lhs.val - rhs);
  }
  friend std::ostream &operator<<(std::ostream &os, const cupvalue &cv) {
    os << cv.val;
    return os;
  }
  friend bool operator==(const cupvalue &lhs, const cupvalue &rhs) {
    return lhs.val == rhs.val;
  }
  // friend std::strong_ordering operator<=>(const cupvalue& lhs, const
  // cupvalue& rhs) {
  //   return lhs.val <=> rhs.val;
  // }
};

auto parseinput(int i) {
  boost::circular_buffer<cupvalue> b(9);
  while (i > 0) {
    b.push_front(cupvalue{i % 10});
    i = i / 10;
  }
  return b;
}

void step(boost::circular_buffer<cupvalue> &buffer, std::size_t current) {
  // fix underflow
  std::size_t target;
  auto seekval = buffer[current] - 1;
  for (std::size_t scan = current + 4; scan < current + buffer.size();
       ++current) {
    if (seekval == buffer[scan]) {
      target = scan;
      break;
    }
  }
  std::array<cupvalue, 3> moving;
  for (std::size_t i = 0; i < 3; ++i) {
    moving[i] = buffer[current + i + 1];
  }
  for (std::size_t m = current + 4; m <= target; ++m) {
    buffer[m - 3] = buffer[m];
  }
  for (std::size_t i = 0; i < 3; ++i) {
    buffer[target - 2 + i] = moving[i];
  }
}

template <typename T> void printcontainer(const T &t) {
  std::cout << '{';
  for (auto e : t) {
    std::cout << e << ", ";
  }
  std::cout << '}' << std::endl;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "USAGE: ./" << argv[0] << " startingsetup_as_integer";
    std::cout << std::endl;
  }
  int input = std::stol(argv[1]);
  auto game = parseinput(input);
  printcontainer(game);
  return 42;
}
