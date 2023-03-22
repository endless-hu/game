#include <assert.h>

#include <iostream>

#include "bit_map.hh"
#include "test_harness.hh"

// test 1
void random_test() {
  TwoDimBitMap bit_map(100, 100);
  std::unordered_set<Point> points;
  // randomly generate a number of points
  for (int i = 0; i < 100; i++) {
    int x = rand() % 100;
    int y = rand() % 100;
    // check if the point is already in the set
    if (points.find(Point(x, y)) != points.end()) {
      continue;
    }
    points.insert(Point(x, y));
    assert(!bit_map.get(x, y));
    bit_map.set(x, y);
    assert(bit_map.get(x, y));
  }

  // check all points in the bitmap
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      if (points.find(Point(i, j)) != points.end()) {
        assert(bit_map.get(i, j));
      } else {
        assert(!bit_map.get(i, j));
      }
    }
  }

  std::cout << "random_test passed!" << std::endl;
}

// test 2: Check the boarder
void boarder_test() {
  TwoDimBitMap bit_map(256, 256);
  // set the top and bottom boarder
  for (int x = 0; x < 256; x++) {
    assert(!bit_map.get(x, 0));
    assert(!bit_map.get(x, 255));
    bit_map.set(x, 0);
    bit_map.set(x, 255);
    assert(bit_map.get(x, 0));
    assert(bit_map.get(x, 255));
  }
  // set the left and right boarder
  for (int y = 0; y < 256; y++) {
    assert(!bit_map.get(0, 0));
    assert(!bit_map.get(255, y));
    bit_map.set(0, y);
    bit_map.set(255, y);
    assert(bit_map.get(0, 0));
    assert(bit_map.get(255, y));
  }

  assert(!bit_map.get(1, 1));

  std::cout << "boarder_test passed!" << std::endl;
}

int main() {
  // seed the random number generator
  srand(time(NULL));
  random_test();
  boarder_test();
  std::cout << "All tests passed" << std::endl;
}