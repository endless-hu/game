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

// ------------------------------
//      Game Implementation
// ------------------------------

Game::Game(int x_size, int y_size) : board(GameBoard(x_size, y_size)) {
  init_sdl();
  cycle = 0;
  running = true;
  // Load font from system default
  TTF_Init();
  font = TTF_OpenFont("../fonts/OpenSans-VariableFont_wdth,wght.ttf", 24);
  if (font == nullptr) {
    std::cout << "Failed to load font" << std::endl;
    std::abort();
  }
}

Game::~Game() {
  SDL_DestroyTexture(cycle_texture);
  SDL_DestroyTexture(start_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}

void Game::init_sdl() {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(
      "Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      board.get_board_size().first * CELL_SIZE + SIDEBAR_WIDTH,
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

  // Load textures for cycle count and start/stop button
  SDL_Color text_color = {255, 255, 255, 255};
  SDL_Surface* surface = TTF_RenderText_Solid(
      font, ("Cycle: \n" + std::to_string(cycle)).c_str(), text_color);
  cycle_texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  if (running) {
    surface = TTF_RenderText_Solid(font, "Stop", text_color);
  } else {
    surface = TTF_RenderText_Solid(font, "Start", text_color);
  }
  start_texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  // draw the side bar and cycle count
  SDL_Rect sidebar_rect = {board.get_board_size().first * CELL_SIZE, 0,
                           SIDEBAR_WIDTH,
                           board.get_board_size().second * CELL_SIZE};
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &sidebar_rect);
  SDL_Rect cycle_rect = {board.get_board_size().first * CELL_SIZE + 10, 10,
                         SIDEBAR_WIDTH - 20, 30};
  SDL_RenderCopy(renderer, cycle_texture, NULL, &cycle_rect);
  // draw the start/stop button
  SDL_Rect start_rect = {board.get_board_size().first * CELL_SIZE + 10,
                         board.get_board_size().second * CELL_SIZE - 40,
                         SIDEBAR_WIDTH - 20, 30};
  SDL_RenderCopy(renderer, start_texture, NULL, &start_rect);

  SDL_RenderPresent(renderer);
}

void Game::run() {
  bool exit = false;
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        exit = true;
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;
        if (x >= board.get_board_size().first * CELL_SIZE &&
            x <= board.get_board_size().first * CELL_SIZE + SIDEBAR_WIDTH) {
          if (y >= board.get_board_size().second * CELL_SIZE - 40 &&
              y <= board.get_board_size().second * CELL_SIZE - 10) {
            running = !running;
          }
        }
      }
    }
    if (exit) {
      break;
    }
    if (running) {
      board.update();
      cycle++;
    }
    render();
    SDL_Delay(DELAY_MS);
  }
}
