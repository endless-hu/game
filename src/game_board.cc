#include "game_board.hh"

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
    // Remain alive if 2 or 3 neighbors are alive
    if (live_neighbors == 2 || live_neighbors == 3) {
      return true;
    } else {
      return false;
    }
  } else {
    // Become alive if 3 neighbors are alive
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

Game::Game(int x_size, int y_size,
           std::vector<void (*)(GameBoard*)> god_functions)
    : board_(GameBoard(x_size, y_size)), god_functions_(god_functions) {
  init_sdl();

  // Load font
  TTF_Init();
  font_ = TTF_OpenFont("../fonts/OpenSans-VariableFont_wdth,wght.ttf", 24);
  if (font_ == nullptr) {
    std::cout << "Failed to load font" << std::endl;
    std::abort();
  }

  // Initialize the game board and necessary variables
  if (god_functions.empty()) {
    board_.randomize();
  }
  cycle_ = 0;
  running_ = false;
}

Game::~Game() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  TTF_CloseFont(font_);
  TTF_Quit();
  SDL_Quit();
}

void Game::init_sdl() {
  SDL_Init(SDL_INIT_VIDEO);
  window_ = SDL_CreateWindow(
      "Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      board_.get_board_size().first * CELL_SIZE + SIDEBAR_WIDTH,
      board_.get_board_size().second * CELL_SIZE, SDL_WINDOW_SHOWN);
  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);

  draw_board();
  draw_sidebar();

  SDL_RenderPresent(renderer_);
}

void Game::draw_sidebar() {
  SDL_Rect sidebar_rect = {board_.get_board_size().first * CELL_SIZE, 0,
                           SIDEBAR_WIDTH,
                           board_.get_board_size().second * CELL_SIZE};
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer_, &sidebar_rect);

  draw_cycle_counts();
  draw_god_function_buttons();
  draw_clear_button();
  draw_start_button();
}

void Game::draw_cycle_counts() {
  SDL_Color text_color = {255, 255, 255, 255};
  SDL_Surface* surface = TTF_RenderText_Solid(
      font_, ("Cycle: \n" + std::to_string(cycle_)).c_str(), text_color);
  SDL_Texture* cycle_texture = SDL_CreateTextureFromSurface(renderer_, surface);
  SDL_FreeSurface(surface);

  SDL_Rect cycle_rect = {board_.get_board_size().first * CELL_SIZE + 10, 10,
                         SIDEBAR_WIDTH - 20, 30};
  SDL_RenderCopy(renderer_, cycle_texture, NULL, &cycle_rect);
  SDL_DestroyTexture(cycle_texture);
}

void Game::draw_god_function_buttons() {
  // Draw the list of god function buttons at the center of the sidebar
  SDL_Color text_color = {255, 255, 255, 255};
  int button_height = 30;
  int button_width = SIDEBAR_WIDTH - 20;
  int button_y = (board_.get_board_size().second * CELL_SIZE -
                  god_functions_.size() * button_height) /
                 2;
  for (int i = 0; i < god_functions_.size(); i++) {
    SDL_Rect button_rect = {board_.get_board_size().first * CELL_SIZE + 10,
                            button_y + i * button_height, button_width,
                            button_height};
    SDL_RenderDrawRect(renderer_, &button_rect);
    SDL_Surface* surface = TTF_RenderText_Solid(
        font_, ("God Function " + std::to_string(i)).c_str(), text_color);
    SDL_Texture* button_texture =
        SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    SDL_Rect button_text_rect = {board_.get_board_size().first * CELL_SIZE + 20,
                                 button_y + i * button_height + 5, button_width,
                                 button_height};
    SDL_RenderCopy(renderer_, button_texture, NULL, &button_text_rect);
    SDL_DestroyTexture(button_texture);
  }
}

void Game::draw_clear_button() {
  // Draw a clear button at the top of start/stop button
  SDL_Color text_color = {0, 255, 0, 255};
  SDL_Rect clear_rect = {
      board_.get_board_size().first * CELL_SIZE + 10,
      board_.get_board_size().second * CELL_SIZE - 2 * CTRL_BUTTON_HIGHT,
      SIDEBAR_WIDTH - 20, CTRL_BUTTON_HIGHT};
  SDL_RenderDrawRect(renderer_, &clear_rect);
  SDL_Surface* surface = TTF_RenderText_Solid(font_, "Clear", text_color);
  SDL_Texture* clear_texture = SDL_CreateTextureFromSurface(renderer_, surface);
  SDL_FreeSurface(surface);
  SDL_RenderCopy(renderer_, clear_texture, NULL, &clear_rect);
  SDL_DestroyTexture(clear_texture);
}

void Game::draw_start_button() {
  // Text color: red
  SDL_Color text_color = {255, 0, 0, 255};
  SDL_Surface* surface;
  if (running_) {
    surface = TTF_RenderText_Solid(font_, "Stop", text_color);
  } else {
    surface = TTF_RenderText_Solid(font_, "Start", text_color);
  }
  SDL_Texture* start_texture = SDL_CreateTextureFromSurface(renderer_, surface);
  SDL_FreeSurface(surface);
  // Place the start/stop button at the bottom of the sidebar
  SDL_Rect start_rect = {
      board_.get_board_size().first * CELL_SIZE + 10,
      board_.get_board_size().second * CELL_SIZE - CTRL_BUTTON_HIGHT,
      SIDEBAR_WIDTH - 20, CTRL_BUTTON_HIGHT};

  SDL_RenderCopy(renderer_, start_texture, NULL, &start_rect);
  SDL_DestroyTexture(start_texture);
}

void Game::draw_board() {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  for (int x = 0; x < board_.get_board_size().first; x++) {
    for (int y = 0; y < board_.get_board_size().second; y++) {
      if (board_.get_cell_state(x, y)) {
        SDL_Rect cell_rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE,
                              CELL_SIZE};
        SDL_RenderFillRect(renderer_, &cell_rect);
      }
    }
  }
}

void Game::run() {
  bool exit = false;
  while (!exit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        exit = true;
      } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        int button_width = SIDEBAR_WIDTH - 20;
        // Clicking on the start/stop button toggles the running state
        int x = event.button.x;
        int y = event.button.y;
        if ((x >= board_.get_board_size().first * CELL_SIZE &&
             x <= board_.get_board_size().first * CELL_SIZE + SIDEBAR_WIDTH) &&
            (y >= board_.get_board_size().second * CELL_SIZE -
                      CTRL_BUTTON_HIGHT &&
             y <= board_.get_board_size().second * CELL_SIZE))
          running_ = !running_;

        // Clicking on the clear button clears the board
        if ((x >= board_.get_board_size().first * CELL_SIZE &&
             x <= board_.get_board_size().first * CELL_SIZE + button_width) &&
            (y >= board_.get_board_size().second * CELL_SIZE -
                      2 * CTRL_BUTTON_HIGHT &&
             y <= board_.get_board_size().second * CELL_SIZE -
                      CTRL_BUTTON_HIGHT)) {
          board_.clear();
          cycle_ = 0;
          running_ = false;
        }

        // Clicking on a god function button runs the corresponding function
        int button_height = 30;
        int button_y = (board_.get_board_size().second * CELL_SIZE -
                        god_functions_.size() * button_height) /
                       2;
        for (int i = 0; i < god_functions_.size(); i++) {
          if ((x >= board_.get_board_size().first * CELL_SIZE &&
               x <= board_.get_board_size().first * CELL_SIZE + button_width) &&
              (y >= button_y + i * button_height &&
               y <= button_y + (i + 1) * button_height)) {
            god_functions_[i](&board_);
            break;
          }
        }
      }
    }
    if (running_) {
      board_.update();
      cycle_++;
    }
    render();
    SDL_Delay(DELAY_MS);
  }
}
