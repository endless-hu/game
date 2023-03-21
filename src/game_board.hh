#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>  // For text rendering

#include <iostream>
#include <sstream>  // For stringstream
#include <string>   // For strings
#include <utility>
#include <vector>

#define CELL_SIZE 10
#define DELAY_MS 100
#define SIDEBAR_WIDTH 200
#define CTRL_BUTTON_HIGHT 100

class GameBoard {
 public:
  GameBoard(int x_size, int y_size);
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

class Game {
 public:
  Game(int board_x_size, int board_y_size,
       std::vector<void (*)(GameBoard*)> god_functions);
  ~Game();     // Destructor to clean up SDL
  void run();  // Run the game loop
 private:
  GameBoard board_;         // The game board
  SDL_Window* window_;      // The SDL window
  SDL_Renderer* renderer_;  // The SDL renderer
  TTF_Font* font_;          // a font to render text

  uint cycle_;    // The current cycle of the game
  bool running_;  // Whether the game is running

  // a set of god functions. The god function takes a GameBoard*
  // as an argument and modifies the board state in some patterns.
  std::vector<void (*)(GameBoard*)> god_functions_;

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