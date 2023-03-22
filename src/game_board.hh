#pragma once

#include <iostream>
#include <memory>
#include <sstream>  // For stringstream
#include <string>   // For strings
#include <utility>
#include <vector>

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
  virtual void update() = 0;
  virtual void clear() = 0;
  virtual void randomize() = 0;

 protected:
  virtual int count_live_neighbors(int x, int y) = 0;
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

  void update();     // Update the board state to the next generation
  void clear();      // Clear the board
  void randomize();  // Randomize the board
 private:
  int x_size_, y_size_;                   // The size of the board
  std::vector<std::vector<bool>> cells_;  // The cells of the board
  // Count the number of live neighbors for a given cell
  int count_live_neighbors(int x, int y);
  // Calculate the next state of a cell at a given position
  bool calculate_next_state(int x, int y);
};

// Optimized implementation of the game board
// Use bit_map to represent the cell states
class OptimizedGameBoard : public AbstractGameBoard {
 public:
  OptimizedGameBoard(int x_size, int y_size);
  std::pair<int, int> get_board_size() const;
  bool get_cell_state(int x, int y) const;
  void set_cell_state(int x, int y, bool state);

  void update();     // Update the board state to the next generation
  void clear();      // Clear the board
  void randomize();  // Randomize the board
 private:
  int x_size_, y_size_;  // The size of the board
  // Count the number of live neighbors for a given cell
  int count_live_neighbors(int x, int y);
  // Calculate the next state of a cell at a given position
  bool calculate_next_state(int x, int y);
};
