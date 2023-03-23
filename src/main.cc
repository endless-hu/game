#include <sys/wait.h>
#include <unistd.h>

#include "game.hh"
// include for std::tie
#include <tuple>

// This god function seeds all cells at the boarder to be alive
void god_function1(AbstractGameBoard* board) {
  int x_size, y_size;
  std::tie(x_size, y_size) = board->get_board_size();
  for (int x = 0; x < x_size; x++) {
    board->set_cell_state(x, 0, true);
    board->set_cell_state(x, y_size - 1, true);
  }
  for (int y = 0; y < y_size; y++) {
    board->set_cell_state(0, y, true);
    board->set_cell_state(x_size - 1, y, true);
  }
}

// This god function seeds life in a normal distribution, i.e. the center of the
// board is more likely to be alive than the edges
void god_function2(AbstractGameBoard* board) {
  int x_size, y_size;
  std::tie(x_size, y_size) = board->get_board_size();
  for (int x = 0; x < x_size; x++) {
    for (int y = 0; y < y_size; y++) {
      double x_center = x_size / 2.0;
      double y_center = y_size / 2.0;
      double x_dist = x - x_center;
      double y_dist = y - y_center;
      double dist = sqrt(x_dist * x_dist + y_dist * y_dist);
      double max_dist = sqrt(x_center * x_center + y_center * y_center);
      double prob = dist / max_dist;
      if (prob > 1.0) {
        prob = 1.0;
      }
      if (prob < 0.0) {
        prob = 0.0;
      }
      double rand = (double)std::rand() / RAND_MAX;
      if (rand > prob) {
        board->set_cell_state(x, y, true);
      }
    }
  }
}

// This god function terminates life in an anti-normal distribution, i.e. the
// edge of the board is more likely to be dead than the center
void god_function3(AbstractGameBoard* board) {
  int x_size, y_size;
  std::tie(x_size, y_size) = board->get_board_size();
  for (int x = 0; x < x_size; x++) {
    for (int y = 0; y < y_size; y++) {
      double x_center = x_size / 2.0;
      double y_center = y_size / 2.0;
      double x_dist = x - x_center;
      double y_dist = y - y_center;
      double dist = sqrt(x_dist * x_dist + y_dist * y_dist);
      double max_dist = sqrt(x_center * x_center + y_center * y_center);
      double prob = dist / max_dist;
      if (prob > 1.0) {
        prob = 1.0;
      }
      if (prob < 0.0) {
        prob = 0.0;
      }
      double rand = (double)std::rand() / RAND_MAX;
      if (rand < prob) {
        board->set_cell_state(x, y, false);
      }
    }
  }
}

void test(int x_size, int y_size, int rounds) {
  std::vector<bool> vec(x_size * y_size);
  for (uint64_t i = 0; i < vec.size(); i++) {
    if (rand() < RAND_MAX / 2) {
      vec[i] = true;
    }
  }
  std::vector<void (*)(AbstractGameBoard*)> god_functions;
  god_functions.push_back(god_function1);
  god_functions.push_back(god_function2);
  god_functions.push_back(god_function3);

  pid_t c_pid = fork();
  if (c_pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if (c_pid == 0) {
    // child process
    AbstractGameBoard* game_board = new OptimizedGameBoard(x_size, y_size);
    Game game(game_board, god_functions, true, 0, rounds);
    game_board->read_state_from(vec);
    game.run();
    std::cout << "Optimized GameBoard occupied "
              << game_board->report_mem_usage() << " bytes of memory."
              << std::endl;
    std::cout << "Optimized GameBoard costs " << game.report_CPU_time() / 1000
              << " ms." << std::endl;
    delete game_board;
    exit(0);
  } else {
    AbstractGameBoard* game_board = new GameBoard(x_size, y_size);
    game_board->read_state_from(vec);
    Game game(game_board, god_functions, true, 0, rounds);
    game.run();
    std::cout << "Unoptimized GameBoard occupied "
              << game_board->report_mem_usage() << " bytes of memory."
              << std::endl;
    std::cout << "Unoptimized GameBoard costs " << game.report_CPU_time() / 1000
              << " ms." << std::endl;
    delete game_board;
  }
  wait(nullptr);
}

int main() {
  srand(10808);  // Set the seed for the random number generator
  std::cout << "------- Verification Test ---------" << std::endl;
  test(256, 256, 100);
  // std::cout << "------- Speed Test ---------" << std::endl;
  // test(2048, 2048, 1000);
}