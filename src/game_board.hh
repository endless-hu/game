#pragma once

#include <iostream>
#include <memory>
#include <sstream>  // For stringstream
#include <string>   // For strings
#include <utility>
#include <vector>

#include "bit_map.hh"

#define NTHR 8

class AbstractGameBoard {
 public:
  virtual ~AbstractGameBoard() = default;
  virtual std::pair<int, int> get_board_size() const = 0;
  virtual bool get_cell_state(int x, int y) const = 0;
  virtual void set_cell_state(int x, int y, bool state) = 0;
  virtual void read_state_from(std::vector<bool>& vec) = 0;
  virtual void update() = 0;  // Update the board state to the next generation
  virtual void clear() = 0;   // Clear the board
  // For test purpose
  virtual int report_mem_usage() = 0;
  bool operator==(const AbstractGameBoard& other) const;
  bool operator!=(const AbstractGameBoard& other) const {
    return !(*this == other);
  }

 protected:
  // Count the number of live neighbors for a given cell
  virtual int count_live_neighbors(int x, int y) = 0;
  // Calculate the next state of a cell at a given position
  virtual bool calculate_next_state(int x, int y) = 0;
};

// Unoptimized implementation of the game board
class GameBoard : public AbstractGameBoard {
 public:
  GameBoard(int x_size, int y_size);
  ~GameBoard() = default;
  std::pair<int, int> get_board_size() const;
  bool get_cell_state(int x, int y) const;
  void set_cell_state(int x, int y, bool state);
  void read_state_from(std::vector<bool>& vec);

  void update();
  void clear();

  int report_mem_usage();

 protected:
  int count_live_neighbors(int x, int y);
  bool calculate_next_state(int x, int y);

 private:
  int x_size_, y_size_;                   // The size of the board
  std::vector<std::vector<bool>> cells_;  // The cells of the board
};

// Optimized implementation of the game board
// Use bit_map to represent the cell states
class OptimizedGameBoard : public AbstractGameBoard {
 public:
  OptimizedGameBoard(int x_size, int y_size);
  std::pair<int, int> get_board_size() const;
  bool get_cell_state(int x, int y) const;
  void set_cell_state(int x, int y, bool state);
  void read_state_from(std::vector<bool>& vec);

  void update();
  void clear();

  int report_mem_usage();

 protected:
  int count_live_neighbors(int x, int y);
  bool calculate_next_state(int x, int y);

 private:
  int x_size_, y_size_;  // The size of the board
  TwoDimBitMap cells_;   // The cells of the board
};

// Fully optimized implementation of the game board
// Beside using bit_map to compress the memory usage, we also use
// multi-threading to speed up the calculation
class FullyOptimizedGameBoard : public AbstractGameBoard {
 public:
  FullyOptimizedGameBoard(int x_size, int y_size);
  std::pair<int, int> get_board_size() const;
  bool get_cell_state(int x, int y) const;
  void set_cell_state(int x, int y, bool state);
  void read_state_from(std::vector<bool>& vec);

  void update();
  void clear();

  int report_mem_usage();

 protected:
  int count_live_neighbors(int x, int y);
  bool calculate_next_state(int x, int y);

 private:
  int x_size_, y_size_;  // The size of the board
  TwoDimBitMap cells_;   // The cells of the board
};
