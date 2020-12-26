#include <boost/circular_buffer.hpp>
#include <boost/circular_buffer/base.hpp>
#include <compare>
#include <iostream>
#include <string>

struct cupvalue {
  int val;
  explicit cupvalue(int v) {
    if (v > 0) {
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

// TODO: add iterable concept
template <typename T> void printcontainer(const T &t) {
  std::cout << '{';
  for (auto e : t) {
    std::cout << e << ", ";
  }
  std::cout << '}' << std::endl;
}

void step(boost::circular_buffer<cupvalue> &buffer, std::size_t current) {
  // fix underflow
  auto seekval = buffer[(current)%9] - 1;
  // TODO: put more descriptive thing for current + buffer.size()
  std::size_t target = current + buffer.size();
  auto seeker = [current, buffer](const cupvalue &seekval) {
    // std::cout << "seeking " << seekval << '\n';
    for (std::size_t scan = current + 4; scan < current + buffer.size();
         ++scan) {
      std::cout << "checking if at position " << scan << " there is a " << seekval << '\n';
      std::cout << "i see a " << buffer[(scan)%9] << "there\n";
      if (seekval == buffer[(scan)%9]) {
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
    std::cout << "searching for " << seekval << '\n';
    std::cout << "current pointer is " << current << '\n';
    target = seeker(seekval);
    std::cout << "target is " << target << '\n';
    std::cout << "target == " << current+buffer.size() << " is the repeat-condition" << std::endl;

    // NB: do not use after loop!
    --seekval;
    // std::cout << " to " << seekval << '\n';
  }
  std::array<cupvalue, 3> moving;
  for (std::size_t i = 0; i < 3; ++i) {
    moving[i] = buffer[(current + i + 1)%9];
  }
  if (seek_n_times>3) {
    std::cout << "this shouldn't have happened" << std::endl;
    std::cout << "started searching at " << buffer[(current)%9] << "-1\n";
    std::cout << "decremented " << seek_n_times << "times\n";
    std::cout << "pick up values are:";
    printcontainer(moving);
    std::cout << "setup is:";
    printcontainer(buffer);
  }

  for (std::size_t m = current + 4; m <= target; ++m) {
    buffer[(m - 3)%9] = buffer[(m)%9];
  }
  for (std::size_t i = 0; i < 3; ++i) {
    buffer[(target - 2 + i)%9] = moving[i];
  }
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
    printcontainer(game);
  }
  return 42;
}
