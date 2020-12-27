// #include <boost/circular_buffer.hpp>
// #include <boost/circular_buffer/base.hpp>
#include <algorithm>
#include <array>
#include <boost/progress.hpp>
#include <compare>
#include <iostream>
#include <string>
#include <vector>

long int CUPVALUES_END = 10;

struct cupvalue {
  long int val;
  explicit operator long() { return val; }
  explicit cupvalue(long int v) {
    if (v == 0) {
      val = CUPVALUES_END - 1;
    } else if (v > 0) {
      val = v % CUPVALUES_END;
    } else {
      long int add_at_least = -v;
      while ((add_at_least % CUPVALUES_END) != 0) {
        add_at_least++;
      }
      if (add_at_least > CUPVALUES_END) {
        std::cout << "huh?\n";
      }
      val = v + add_at_least;
      if (val < 0 || val >= CUPVALUES_END) {
        std::cout << "BIG mistake 2\n";
      }
    }
  }
  cupvalue() {}
  cupvalue &operator=(const cupvalue &) = default;
  cupvalue &operator=(cupvalue &&) = default;
  cupvalue(const cupvalue &) = default;
  cupvalue(cupvalue &&) = default;
  friend cupvalue operator-(const cupvalue &lhs, long int rhs) {
    return cupvalue(lhs.val - rhs);
  }
  cupvalue &operator--() {
    if (val == 0) {
      val = CUPVALUES_END - 1;
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
  CUPVALUES_END = 10;
  std::vector<cupvalue> reverse;
  std::vector<cupvalue> b;
  while (i > 0) {
    reverse.push_back(cupvalue{i % 10});
    i = i / 10;
  }
  for (auto it = reverse.rbegin(); it != reverse.rend(); it++) {
    b.push_back(*it);
  }
  // part 2
  long int next_insert = 10;
  b.reserve(1000000);
  CUPVALUES_END = 1000100; // a bit generous
  while (b.size() < 1000000) {
    b.push_back(cupvalue(next_insert++));
  }
  CUPVALUES_END = next_insert;
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

std::size_t percent(std::size_t a, std::size_t b) {
  if (a < b)
    return a;
  a -= b;
  if (a < b)
    return a;
  return a % b;
}

template <typename T> void step(T &buffer, std::size_t current) {
  current = percent(current, buffer.size());
  // fix underflow
  auto seekval = buffer[percent(current, buffer.size())] - 1;
  // TODO: put more descriptive thing for current + buffer.size()
  std::size_t target = current + buffer.size();
  auto seeker = [current, buffer](const cupvalue &seekval) {
    // std::cout << "seeking " << seekval << '\n';
    // for (std::size_t scan = current + 4; scan < current + buffer.size();
    //      ++scan) {
    for (std::size_t scan = current + buffer.size() - 1; scan >= current + 4;
         --scan) {
      // std::cout << "checking if at position " << scan << " there is a " <<
      // seekval << '\n'; std::cout << "i see a " <<
      // buffer[(scan)%buffer.size()] << "there\n";
      if (seekval == buffer[percent(scan, buffer.size())]) {
        // target = scan;
        return scan;
      }
    }
    // std::cout << "nope\n";
    return current + buffer.size();
  };

  std::array<cupvalue, 3> moving;
  for (std::size_t i = 0; i < 3; ++i) {
    moving[i] = buffer[percent(current + i + 1, buffer.size())];
  }

  int seek_n_times = 1;
  while (std::find(moving.begin(), moving.end(), seekval) != moving.end()) {
    seek_n_times++;
    --seekval;
  }
  if (seek_n_times > 3) {
    std::cout << "this shouldn't have happened" << std::endl;
    std::cout << "started searching at "
              << buffer[percent(current, buffer.size())] << "-1\n";
    std::cout << "decremented " << seek_n_times << "times\n";
    std::cout << "pick up values are:";
    // printcontainer(moving);
    std::cout << "setup is:";
    // printcontainer(buffer);
  }

  int actual_seeks = 0;
  while (target == current + buffer.size()) {
    actual_seeks++;
    // std::cout << "searching for " << seekval << '\n';
    // std::cout << "current pointer is " << current << '\n';
    target = seeker(seekval);
    // std::cout << "target is " << target << '\n';
    // std::cout << "target == " << current+buffer.size() << " is the
    // repeat-condition" << std::endl;

    // NB: do not use after loop!
    --seekval;
    // std::cout << " to " << seekval << '\n';
    if (actual_seeks > 1) {
      std::cout << "this shouldn't have happened, either" << std::endl;
      std::cout << "started searching at "
                << buffer[percent(current, buffer.size())] << "-1\n";
      std::cout << "searched " << actual_seeks << " times\n";
      std::cout << "pick up values are:";
      // printcontainer(moving);
      std::cout << "setup is:";
      // printcontainer(buffer);
    }
  }

  {
    if (target > 2 * buffer.size()) {
      std::cout << "optimization assumption violated\n";
    }
    std::size_t m;
    // source and destination don't need modulo
    for (m = current + 4; m <= target && m < buffer.size(); ++m) {
      buffer[m - 3] = buffer[m];
    }
    // source needs modulo, destination doesn't
    for (; m <= target && (m - 3) < buffer.size(); ++m) {
      buffer[percent(m - 3, buffer.size())] = buffer[percent(m, buffer.size())];
    }
    // source and destination need one modulo
    for (; m <= target; ++m) {
      buffer[(m - 3) - buffer.size()] = buffer[(m)-buffer.size()];
    }
  }
  for (std::size_t i = 0; i < 3; ++i) {
    buffer[percent(target - 2 + i, buffer.size())] = moving[i];
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
  boost::progress_display prog(10000000);
  // printcontainer(game);
  for (std::size_t steps = 0; steps < 10000000; steps++) {
    step(game, steps);
    ++prog;
    // printcontainer(game);
  }
  // printcontainer(game);
  auto one = std::find(game.begin(), game.end(), cupvalue{1});
  if (one == game.end()) {
    std::cout << "messed up\n";
  }
  if (one + 1 == game.end()) {
    std::cout << "a little difficult\n";
  } else {
    std::cout << "number after 1 is " << *(one + 1) << '\n';
  }
  if (one + 2 == game.end()) {
    std::cout << "a little harder\n";
  } else {
    std::cout << "number two after 1 is " << *(one + 2) << '\n';
  }
  std::cout << "answer is " << long(*(one + 1)) * long(*(one + 2)) << '\n';
  return 42;
}
