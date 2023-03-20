#pragma once

#include <SDL2/SDL.h>

#include <utility>
#include <vector>

#define CELL_SIZE 10
#define DELAY_MS 100

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
  void run();                                // Run the game loop
 private:
  GameBoard board;         // The game board
  SDL_Window* window;      // The SDL window
  SDL_Renderer* renderer;  // The SDL renderer
  void init_sdl();         // Initialize SDL
  void render();           // Render the current board state
};