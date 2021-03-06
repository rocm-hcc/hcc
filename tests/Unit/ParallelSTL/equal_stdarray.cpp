
// RUN: %hc %s -o %t.out && %t.out

// Parallel STL headers
#include <coordinate>
#include <experimental/algorithm>
#include <experimental/execution_policy>

#define _DEBUG (0)
#include "test_base.h"

// test equal (non-predicated version)
template<typename T, size_t SIZE>
bool test(void) {

  using std::experimental::parallel::par;

  bool ret = true;
  bool eq = true;

  // std::array
  typedef std::array<T, SIZE> stdArray;
  // non-predicated equal
  ret &= run_and_compare<T, SIZE, stdArray>([&eq]
                                            (stdArray &input1, stdArray &input2, stdArray &output1,
                                                                                 stdArray &output2) {
    auto expected = std::equal(std::begin(input1), std::end(input1), std::begin(input2));
    auto result   = std::experimental::parallel::
                    equal(par, std::begin(input1), std::end(input1), std::begin(input2));

    eq = EQ(expected, result);
  }, false);
  ret &= eq;

  // predicated equal

  auto pred = [](const T& a, const T& b) { return ((a + 1) == b); };

  ret &= run_and_compare<T, SIZE, stdArray>([&eq, pred]
                                            (stdArray &input1, stdArray &input2, stdArray &output1,
                                                                                 stdArray &output2) {
    auto expected = std::equal(std::begin(input1), std::end(input1), std::begin(input2), pred);
    auto result   = std::experimental::parallel::
                    equal(par, std::begin(input1), std::end(input1), std::begin(input2), pred);

    eq = EQ(expected, result);
  }, false);
  ret &= eq;

  return ret;
}

int main() {
  bool ret = true;

  ret &= test<int, TEST_SIZE>();
  ret &= test<unsigned, TEST_SIZE>();
  ret &= test<float, TEST_SIZE>();
  ret &= test<double, TEST_SIZE>();

  return !(ret == true);
}

