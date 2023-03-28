#include "test_harness.hh"

void test_game_board(int x_size, int y_size, int rounds) {
  std::vector<bool> vec(x_size * y_size);
  for (uint64_t i = 0; i < vec.size(); i++) {
    if (rand() < RAND_MAX / 2) {
      vec[i] = true;
    }
  }
  AbstractGameBoard* game_board = new GameBoard(x_size, y_size);
  AbstractGameBoard* alternative_game_board =
      new OptimizedGameBoard(x_size, y_size);
  game_board->read_state_from(vec);
  alternative_game_board->read_state_from(vec);
  GameBoardTester tester(game_board, alternative_game_board);
  tester.run(rounds, {});
  delete game_board;
  delete alternative_game_board;
}

int main() { test_game_board(256, 256, 100); }
