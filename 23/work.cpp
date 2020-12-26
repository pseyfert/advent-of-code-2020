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
      while ((add_at_least % 10) != 0) {
        add_at_least++;
      }
      if (add_at_least > 10) {
        std::cout << "huh?\n";
      }
      val = v + add_at_least;
      if (val < 0 || val >= 10) {
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
  cupvalue &operator--() {
    if (val == 0) {
      val = 9;
    } else {
      val--;
    }
    return *this;
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
  auto seekval = buffer[current] - 1;
  // TODO: put more descriptive thing for current + buffer.size()
  std::size_t target = current + buffer.size();
  auto seeker = [current, buffer](const cupvalue &seekval) {
    // std::cout << "seeking " << seekval << '\n';
    for (std::size_t scan = current + 4; scan < current + buffer.size();
         ++scan) {
      if (seekval == buffer[scan]) {
        // target = scan;
        return scan;
      }
    }
    // std::cout << "nope\n";
    return current + buffer.size();
  };
  int seek_n_times = 0;
  while (target == current + buffer.size()) {
    seek_n_times++;
    target = seeker(seekval);
    // NB: do not use after loop!
    // std::cout << "reducing seekval from " << seekval;
    --seekval;
    // std::cout << " to " << seekval << '\n';
  }
  if (seek_n_times>3) {
    std::cout << "this shouldn't have happened" << std::endl;
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
    std::cout << "USAGE: " << argv[0] << " startingsetup_as_integer";
    std::cout << std::endl;
    return 666;
  }
  int input = std::stol(argv[1]);
  auto game = parseinput(input);
  printcontainer(game);
  for (std::size_t steps = 0; steps < 100; steps++) {
    step(game, steps);
    // printcontainer(game);
  }
  return 42;
}
