#include "game_board.hh"

GameBoard::GameBoard(int x_size, int y_size) {
  this->x_size = x_size;
  this->y_size = y_size;
  board = new bool*[x_size];
  for (int i = 0; i < x_size; i++) {
    board[i] = new bool[y_size];
  }
}

GameBoard::~GameBoard() {
  for (int i = 0; i < x_size; i++) {
    delete[] board[i];
  }
  delete[] board;
}

void GameBoard::set_cell_state(int x, int y, bool state) {
  board[x][y] = state;
}

bool GameBoard::get_cell_state(int x, int y) { return board[x][y]; }

std::pair<int, int> GameBoard::get_board_size() {
  return std::make_pair(x_size, y_size);
}

void GameBoard::update() {
  bool** new_board = new bool*[x_size];
  for (int i = 0; i < x_size; i++) {
    new_board[i] = new bool[y_size];
  }
  for (int i = 0; i < x_size; i++) {
    for (int j = 0; j < y_size; j++) {
      new_board[i][j] = calculate_next_state(i, j);
    }
  }
  for (int i = 0; i < x_size; i++) {
    delete[] board[i];
  }
  delete[] board;
  board = new_board;
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
      if (new_x < 0 || new_x >= x_size || new_y < 0 || new_y >= y_size) {
        continue;
      }
      if (board[new_x][new_y]) {
        count++;
      }
    }
  }
  return count;
}

bool GameBoard::calculate_next_state(int x, int y) {
  int live_neighbors = count_live_neighbors(x, y);
  if (board[x][y]) {
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
