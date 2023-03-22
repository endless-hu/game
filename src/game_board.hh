#pragma once

#include <iostream>
#include <memory>
#include <sstream>  // For stringstream
#include <string>   // For strings
#include <utility>
#include <vector>

#include "bit_map.hh"

#define CELL_SIZE 10
#define DELAY_MS 100
#define SIDEBAR_WIDTH 200
#define CTRL_BUTTON_HIGHT 100

class AbstractGameBoard {
 public:
  virtual ~AbstractGameBoard() = default;
  virtual std::pair<int, int> get_board_size() const = 0;
  virtual bool get_cell_state(int x, int y) const = 0;
  virtual void set_cell_state(int x, int y, bool state) = 0;
  virtual void update() = 0;  // Update the board state to the next generation
  virtual void clear() = 0;   // Clear the board

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

  void update();
  void clear();

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

  void update();
  void clear();

 protected:
  int count_live_neighbors(int x, int y);
  bool calculate_next_state(int x, int y);

 private:
  int x_size_, y_size_;  // The size of the board
  TwoDimBitMap cells_;   // The cells of the board
};
