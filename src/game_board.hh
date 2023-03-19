#pragma once

#include <SDL2/SDL.h>

#include <utility>

class GameBoard {
 public:
  GameBoard(
      int x_size,
      int y_size);  // Constructor to initialize the board with a given size
  ~GameBoard();     // Destructor to release memory
  void set_cell_state(
      int x, int y, bool state);  // Set the state of a cell at a given position
  bool get_cell_state(int x,
                      int y);  // Get the state of a cell at a given position
  std::pair<int, int> get_board_size();  // Get the size of the board
  void update();  // Update the board to the next generation
 private:
  bool** board;  // The 2D array to hold the board
  int x_size;
  int y_size;  // The size of the board
  int count_live_neighbors(
      int x, int y);  // Count the number of live neighbors for a given cell
  bool calculate_next_state(
      int x, int y);  // Calculate the next state of a cell at a given position
};

class Game {
 public:
  Game(int board_x_size, int board_y_size);  // Constructor to initialize the
                                             // game with a given board size
  void run();                                // Run the game loop
 private:
  GameBoard board;         // The game board
  SDL_Window* window;      // The SDL window
  SDL_Renderer* renderer;  // The SDL renderer
  void init_sdl();         // Initialize SDL
  void render();           // Render the current board state
};