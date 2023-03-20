#include "game_board.hh"

GameBoard::GameBoard(int x_size, int y_size)
    : x_size_(x_size),
      y_size_(y_size),
      cells_(x_size, std::vector<bool>(y_size, false)) {
  // Randomly assign the initial state of the board
  randomize();
}

void GameBoard::clear() {
  for (int x = 0; x < x_size_; x++) {
    for (int y = 0; y < y_size_; y++) {
      cells_[x][y] = false;
    }
  }
}

void GameBoard::randomize() {
  for (int x = 0; x < x_size_; x++) {
    for (int y = 0; y < y_size_; y++) {
      cells_[x][y] = rand() % 2;
    }
  }
}

void GameBoard::set_cell_state(int x, int y, bool state) {
  cells_[x][y] = state;
}

bool GameBoard::get_cell_state(int x, int y) const { return cells_[x][y]; }

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
    if (live_neighbors < 2) {
      return false;
    } else if (live_neighbors == 2 || live_neighbors == 3) {
      return true;
    } else {
      return false;
    }
  } else {
    if (live_neighbors == 3) {
      return true;
    } else {
      return false;
    }
  }
}

void Game::init_sdl() {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(
      "Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      board.get_board_size().first * CELL_SIZE,
      board.get_board_size().second * CELL_SIZE, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int x = 0; x < board.get_board_size().first; x++) {
    for (int y = 0; y < board.get_board_size().second; y++) {
      if (board.get_cell_state(x, y)) {
        SDL_Rect cell_rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE,
                              CELL_SIZE};
        SDL_RenderFillRect(renderer, &cell_rect);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

Game::Game(int x_size, int y_size) : board(GameBoard(x_size, y_size)) {
  init_sdl();
}

void Game::run() {
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    board.update();
    render();

    SDL_Delay(DELAY_MS);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

/*
Game::Game(int board_x_size, int board_y_size)
    : board(board_x_size, board_y_size) {
  init_sdl();
}

void Game::init_sdl() {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  std::pair<int, int> board_size = board.get_board_size();
  int cell_width = 640 / board_size.first;
  int cell_height = 480 / board_size.second;
  for (int i = 0; i < board_size.first; i++) {
    for (int j = 0; j < board_size.second; j++) {
      if (board.get_cell_state(i, j)) {
        SDL_Rect rect;
        rect.x = i * cell_width;
        rect.y = j * cell_height;
        rect.w = cell_width;
        rect.h = cell_height;
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  SDL_RenderPresent(renderer);
}

void Game::run() {
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }
    render();
    board.update();
    SDL_Delay(100);
  }
}
*/
