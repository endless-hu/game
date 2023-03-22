#pragma once

#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <vector>

/**
 * A bit map class. Specify the number of bits in the template parameter.
 * IT IS ZERO-INDEXED!
 * The index of the class:
 * | HHHH | HHHH | HHHH | HHHH |
 * +------+------+------+------+
 *   [0]     [1]    [2]    [3]
 *  63..0  127..64
 * */
class BitMap {
 public:
  BitMap(int size) : bits_((size - 1) / 64 + 1) {}
  ~BitMap() = default;

  // Set the bit at position i to 1
  void set(int i) {
    assert(static_cast<size_t>(i) < bits_.size() * 64);
    bits_[i / 64] |= (1UL << (i % 64));
  }

  // Set the bit at position i to 0
  void clear(int i) {
    assert(static_cast<uint64_t>(i) < bits_.size() * 64);
    bits_[i / 64] &= ~(1UL << (i % 64));
  }

  // Get the value of the bit at position i
  bool get(int i) const {
    assert(static_cast<uint64_t>(i) < bits_.size() * 64);
    return bits_[i / 64] & (1UL << (i % 64));
  }

  // Clear the bit map
  void clear() {
    for (auto& bit : bits_) bit = 0UL;
  }

 private:
  std::vector<uint64_t> bits_;
};

class TwoDimBitMap {
 public:
  TwoDimBitMap(int x_size, int y_size) {
    bits_.resize((x_size - 1) / 64 + 1, BitMap(y_size));
  }
  ~TwoDimBitMap() = default;

  // Set the bit at position (i, j) to 1
  void set(int i, int j) {
    assert(static_cast<uint64_t>(i) < bits_.size() * 64);
    bits_[i / 64].set(j);
  }

  // Set the bit at position (i, j) to 0
  void clear(int i, int j) {
    assert(static_cast<uint64_t>(i) < bits_.size() * 64);
    bits_[i / 64].clear(j);
  }

  // Get the value of the bit at position (i, j)
  bool get(int i, int j) const {
    assert(static_cast<uint64_t>(i) < bits_.size() * 64);
    return bits_[i / 64].get(j);
  }

  // Clear the bit map
  void clear() {
    for (auto& bit_map : bits_) bit_map.clear();
  }

 private:
  std::vector<BitMap> bits_;
};