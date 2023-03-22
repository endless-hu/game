#include <assert.h>

#include <iostream>
#include <unordered_set>

#include "bit_map.hh"

class Point {
 public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}
  ~Point() = default;

  int x() const { return x_; }
  int y() const { return y_; }

  bool operator==(const Point& other) const {
    return x_ == other.x_ && y_ == other.y_;
  }

 private:
  int x_;
  int y_;
};

namespace std {
template <>
struct hash<Point> {
  size_t operator()(const Point& p) const {
    size_t h1 = std::hash<int>()(p.x());
    size_t h2 = std::hash<int>()(p.y());
    return h1 ^ (h2 << 1);
  }
};
}  // namespace std

int main() {
  // seed the random number generator
  srand(time(NULL));
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

  std::cout << "Test passed!" << std::endl;
}