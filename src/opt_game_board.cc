#include "game_board.hh"

OptimizedGameBoard::OptimizedGameBoard(int x_size, int y_size)
    : x_size_(x_size), y_size_(y_size), cells_(x_size, y_size) {}

std::pair<int, int> OptimizedGameBoard::get_board_size() const {
  return std::make_pair(x_size_, y_size_);
}

bool OptimizedGameBoard::get_cell_state(int x, int y) const {
  return cells_.get(x, y);
}

void OptimizedGameBoard::set_cell_state(int x, int y, bool state) {
  if (state) {
    cells_.set(x, y);
  } else {
    cells_.clear(x, y);
  }
}

void OptimizedGameBoard::update() {
  TwoDimBitMap next_cells(x_size_, y_size_);
  for (int i = 0; i < x_size_; i++) {
    for (int j = 0; j < y_size_; j++) {
      if (calculate_next_state(i, j)) {
        next_cells.set(i, j);
      } else {
        next_cells.clear(i, j);
      }
    }
  }
  cells_ = next_cells;
}

void OptimizedGameBoard::clear() { cells_.clear(); }

int OptimizedGameBoard::count_live_neighbors(int x, int y) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0) {
        continue;
      }
      int x_pos = x + i;
      int y_pos = y + j;
      if (x_pos < 0) {
        x_pos = x_size_ - 1;
      } else if (x_pos >= x_size_) {
        x_pos = 0;
      }
      if (y_pos < 0) {
        y_pos = y_size_ - 1;
      } else if (y_pos >= y_size_) {
        y_pos = 0;
      }
      if (cells_.get(x_pos, y_pos)) {
        count++;
      }
    }
  }
  return count;
}

bool OptimizedGameBoard::calculate_next_state(int x, int y) {
  int live_neighbors = count_live_neighbors(x, y);
  if (cells_.get(x, y)) {
    return live_neighbors == 2 || live_neighbors == 3;
  } else {
    return live_neighbors == 3;
  }
}
