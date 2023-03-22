#pragma once

#include <cstdint>
#include <unordered_set>

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