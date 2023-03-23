#include "game.hh"

Game::Game(AbstractGameBoard* board,
           std::vector<void (*)(AbstractGameBoard*)> god_functions,
           bool running, int init_with_god)
    : board_(board),
      cycle_(0),
      running_(running),
      god_functions_(god_functions) {
  init_sdl();

  // Load font
  TTF_Init();
  font_ = TTF_OpenFont("../fonts/OpenSans-VariableFont_wdth,wght.ttf", 24);
  if (font_ == nullptr) {
    std::cout << "Failed to load font" << std::endl;
    std::abort();
  }

  if (init_with_god && !god_functions_.empty()) {
    god_functions_[init_with_god % god_functions_.size()](board_);
  }
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
      board_->get_board_size().first * CELL_SIZE + SIDEBAR_WIDTH,
      board_->get_board_size().second * CELL_SIZE, SDL_WINDOW_SHOWN);
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
  SDL_Rect sidebar_rect = {board_->get_board_size().first * CELL_SIZE, 0,
                           SIDEBAR_WIDTH,
                           board_->get_board_size().second * CELL_SIZE};
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

  SDL_Rect cycle_rect = {board_->get_board_size().first * CELL_SIZE + 10, 10,
                         SIDEBAR_WIDTH - 20, 30};
  SDL_RenderCopy(renderer_, cycle_texture, NULL, &cycle_rect);
  SDL_DestroyTexture(cycle_texture);
}

void Game::draw_god_function_buttons() {
  // Draw the list of god function buttons at the center of the sidebar
  SDL_Color text_color = {255, 255, 255, 255};
  int button_height = 30;
  int button_width = SIDEBAR_WIDTH - 20;
  int button_y = (board_->get_board_size().second * CELL_SIZE -
                  god_functions_.size() * button_height) /
                 2;
  for (int i = 0; i < static_cast<int>(god_functions_.size()); i++) {
    SDL_Rect button_rect = {board_->get_board_size().first * CELL_SIZE + 10,
                            button_y + i * button_height, button_width,
                            button_height};
    SDL_RenderDrawRect(renderer_, &button_rect);
    SDL_Surface* surface = TTF_RenderText_Solid(
        font_, ("God Function " + std::to_string(i)).c_str(), text_color);
    SDL_Texture* button_texture =
        SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    SDL_Rect button_text_rect = {
        board_->get_board_size().first * CELL_SIZE + 20,
        button_y + i * button_height + 5, button_width, button_height};
    SDL_RenderCopy(renderer_, button_texture, NULL, &button_text_rect);
    SDL_DestroyTexture(button_texture);
  }
}

void Game::draw_clear_button() {
  // Draw a clear button at the top of start/stop button
  SDL_Color text_color = {0, 255, 0, 255};
  SDL_Rect clear_rect = {
      board_->get_board_size().first * CELL_SIZE + 10,
      board_->get_board_size().second * CELL_SIZE - 2 * CTRL_BUTTON_HIGHT,
      SIDEBAR_WIDTH - 20, CTRL_BUTTON_HIGHT};
  SDL_RenderDrawRect(renderer_, &clear_rect);
  SDL_Surface* surface = TTF_RenderText_Solid(font_, "Clear", text_color);
  SDL_Texture* clear_texture = SDL_CreateTextureFromSurface(renderer_, surface);
  SDL_FreeSurface(surface);
  SDL_RenderCopy(renderer_, clear_texture, NULL, &clear_rect);
  SDL_DestroyTexture(clear_texture);
  // Add a border to the start/stop button, the boarder color is azure
  SDL_SetRenderDrawColor(renderer_, 0, 127, 255, 255);
  SDL_RenderDrawRect(renderer_, &clear_rect);
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
      board_->get_board_size().first * CELL_SIZE + 10,
      board_->get_board_size().second * CELL_SIZE - CTRL_BUTTON_HIGHT,
      SIDEBAR_WIDTH - 20, CTRL_BUTTON_HIGHT};

  SDL_RenderCopy(renderer_, start_texture, NULL, &start_rect);
  SDL_DestroyTexture(start_texture);
  // Add a border to the start/stop button, the boarder color is azure
  SDL_SetRenderDrawColor(renderer_, 0, 127, 255, 255);
  SDL_RenderDrawRect(renderer_, &start_rect);
}

void Game::draw_board() {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  for (int x = 0; x < board_->get_board_size().first; x++) {
    for (int y = 0; y < board_->get_board_size().second; y++) {
      if (board_->get_cell_state(x, y)) {
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
        if ((x >= board_->get_board_size().first * CELL_SIZE &&
             x <= board_->get_board_size().first * CELL_SIZE + SIDEBAR_WIDTH) &&
            (y >= board_->get_board_size().second * CELL_SIZE -
                      CTRL_BUTTON_HIGHT &&
             y <= board_->get_board_size().second * CELL_SIZE))
          running_ = !running_;

        // Clicking on the clear button clears the board
        if ((x >= board_->get_board_size().first * CELL_SIZE &&
             x <= board_->get_board_size().first * CELL_SIZE + button_width) &&
            (y >= board_->get_board_size().second * CELL_SIZE -
                      2 * CTRL_BUTTON_HIGHT &&
             y <= board_->get_board_size().second * CELL_SIZE -
                      CTRL_BUTTON_HIGHT)) {
          board_->clear();
          cycle_ = 0;
          running_ = false;
        }

        // Clicking on a god function button runs the corresponding function
        int button_height = 30;
        int button_y = (board_->get_board_size().second * CELL_SIZE -
                        god_functions_.size() * button_height) /
                       2;
        for (int i = 0; i < static_cast<int>(god_functions_.size()); i++) {
          if ((x >= board_->get_board_size().first * CELL_SIZE &&
               x <=
                   board_->get_board_size().first * CELL_SIZE + button_width) &&
              (y >= button_y + i * button_height &&
               y <= button_y + (i + 1) * button_height)) {
            god_functions_[i](board_);
            break;
          }
        }
      }
    }
    if (running_) {
      board_->update();
      cycle_++;
      // auto stop at cycle 100
      if (cycle_ == 100) {
        running_ = false;
      }
    }
    render();
    SDL_Delay(DELAY_MS);
  }
}
