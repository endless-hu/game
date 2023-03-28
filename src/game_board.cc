#include "game_board.hh"

bool AbstractGameBoard::operator==(const AbstractGameBoard& other) const {
  auto [x_size, y_size] = get_board_size();
  if (x_size != other.get_board_size().first ||
      y_size != other.get_board_size().second) {
    return false;
  }
  for (int x = 0; x < x_size; x++) {
    for (int y = 0; y < y_size; y++) {
      if (get_cell_state(x, y) != other.get_cell_state(x, y)) {
        return false;
      }
    }
  }
  return true;
}

GameBoard::GameBoard(int x_size, int y_size)
    : x_size_(x_size),
      y_size_(y_size),
      cells_(x_size, std::vector<bool>(y_size, false)) {}

void GameBoard::clear() {
  for (int x = 0; x < x_size_; x++) {
    for (int y = 0; y < y_size_; y++) {
      cells_[x][y] = false;
    }
  }
}

void GameBoard::set_cell_state(int x, int y, bool state) {
  cells_[x][y] = state;
}

bool GameBoard::get_cell_state(int x, int y) const { return cells_[x][y]; }

void GameBoard::read_state_from(std::vector<bool>& vec) {
  assert(vec.size() == (uint64_t)(x_size_ * y_size_));
  int idx = 0;
  for (int i = 0; i < x_size_; i++) {
    for (int j = 0; j < y_size_; j++) {
      cells_[i][j] = vec[idx++];
    }
  }
}

std::pair<int, int> GameBoard::get_board_size() const {
  return std::make_pair(x_size_, y_size_);
}

void GameBoard::update() {
  std::vector<std::vector<bool>> new_cells(x_size_, std::vector<bool>(y_size_));
  for (int x = 0; x < x_size_; x++) {
    for (int y = 0; y < y_size_; y++) {
      new_cells[x][y] = calculate_next_state(x, y);
    }
  }
  cells_ = new_cells;
}

int GameBoard::count_live_neighbors(int x, int y) {
  int count = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0) {
        continue;
      }
      int new_x = x + i;
      int new_y = y + j;
      if (new_x < 0 || new_x >= x_size_ || new_y < 0 || new_y >= y_size_) {
        continue;
      }
      if (cells_[new_x][new_y]) {
        count++;
      }
    }
  }
  return count;
}

bool GameBoard::calculate_next_state(int x, int y) {
  int live_neighbors = count_live_neighbors(x, y);
  if (cells_[x][y]) {
    return live_neighbors == 2 || live_neighbors == 3;
  } else {
    return live_neighbors == 3;
  }
}

int GameBoard::report_mem_usage() { return x_size_ * y_size_ * sizeof(bool); }

// ---------------------------------------------------------------------------
//                   OptimizedGameBoard
// ---------------------------------------------------------------------------

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

void OptimizedGameBoard::read_state_from(std::vector<bool>& vec) {
  assert(vec.size() == (uint64_t)(x_size_ * y_size_));
  int idx = 0;
  for (int i = 0; i < x_size_; i++) {
    for (int j = 0; j < y_size_; j++) {
      if (vec[idx++]) {
        cells_.set(i, j);
      }
    }
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
      int new_x = x + i;
      int new_y = y + j;
      if (new_x < 0 || new_x >= x_size_ || new_y < 0 || new_y >= y_size_) {
        continue;
      }
      if (cells_.get(new_x, new_y)) {
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
  }
  return live_neighbors == 3;
}

int OptimizedGameBoard::report_mem_usage() {
  return cells_.report_memory_usage();
}
