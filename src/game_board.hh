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

class GameBoard {
 public:
  GameBoard(
      int x_size,
      int y_size);  // Constructor to initialize the board with a given size
  std::pair<int, int> get_board_size() const;  // Get the size of the board
  bool get_cell_state(
      int x, int y) const;  // Get the state of a cell at a given position
  void set_cell_state(
      int x, int y, bool state);  // Set the state of a cell at a given position
  void update();     // Update the board state to the next generation
  void clear();      // Clear the board
  void randomize();  // Randomize the board
  // void god();  // Implement a god function to modify the board state randomly
 private:
  int x_size_, y_size_;                   // The size of the board
  std::vector<std::vector<bool>> cells_;  // The cells of the board
  int count_live_neighbors(
      int x, int y);  // Count the number of live neighbors for a given cell
  bool calculate_next_state(
      int x, int y);  // Calculate the next state of a cell at a given position
};

class Game {
 public:
  Game(int board_x_size, int board_y_size);  // Constructor to initialize the
                                             // game with a given board size
  ~Game();                                   // Destructor to clean up SDL
  void run();                                // Run the game loop
 private:
  GameBoard board_;             // The game board
  SDL_Window* window_;          // The SDL window
  SDL_Renderer* renderer_;      // The SDL renderer
  TTF_Font* font_;              // a font to render text
  SDL_Texture* cycle_texture_;  // Texture for the cycle count
  SDL_Texture* start_texture_;  // Texture for the start/stop button

  uint cycle_;    // The current cycle of the game
  bool running_;  // Whether the game is running

  void init_sdl();        // Initialize SDL
  void render();          // Render the current board state
  void render_sidebar();  // Render the sidebar with cycle count and button
  void handle_events();   // Handle SDL events
};