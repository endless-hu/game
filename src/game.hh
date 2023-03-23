#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>  // For text rendering

#include "game_board.hh"

#define CELL_SIZE 5
#define DELAY_MS 100
#define SIDEBAR_WIDTH 200
#define CTRL_BUTTON_HIGHT 100

class Game {
 public:
  explicit Game(AbstractGameBoard* board,
                std::vector<void (*)(AbstractGameBoard*)> god_functions,
                bool running, int init_with_god);
  ~Game();     // Destructor to clean up SDL
  void run();  // Run the game loop
  int report_game_board_mem_usage();

 private:
  AbstractGameBoard* board_;  // The game board
  SDL_Window* window_;        // The SDL window
  SDL_Renderer* renderer_;    // The SDL renderer
  TTF_Font* font_;            // a font to render text

  uint cycle_;    // The current cycle of the game
  bool running_;  // Whether the game is running

  // a set of god functions. The god function takes a GameBoard*
  // as an argument and modifies the board state in some patterns.
  std::vector<void (*)(AbstractGameBoard*)> god_functions_;

  void init_sdl();        // Initialize SDL
  void render();          // Render the current board state
  void render_sidebar();  // Render the sidebar with cycle count and button
  void handle_events();   // Handle SDL events

  void draw_board();
  void draw_sidebar();
  void draw_cycle_counts();
  void draw_god_function_buttons();
  void draw_clear_button();
  void draw_start_button();
};