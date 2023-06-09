#pragma once

#include <chrono>
#include <cstdint>
#include <unordered_set>
#include <vector>

#include "game_board.hh"

class Point {
 public:
  Point() = default;
  Point(int x, int y) : x_(x), y_(y) {}
  ~Point() = default;

  int x() const { return x_; }
  int y() const { return y_; }

  bool operator==(const Point& other) const {
    return x_ == other.x_ && y_ == other.y_;
  }

 private:
  int x_;
  int y_;
};

namespace std {
template <>
struct hash<Point> {
  size_t operator()(const Point& p) const {
    size_t h1 = std::hash<int>()(p.x());
    size_t h2 = std::hash<int>()(p.y());
    return h1 ^ (h2 << 1);
  }
};
}  // namespace std

class GameBoardTester {
 public:
  GameBoardTester(AbstractGameBoard* game_board,
                  AbstractGameBoard* alternative_game_board)
      : game_board_(game_board),
        alternative_game_board_(alternative_game_board) {
    if (*game_board_ != *alternative_game_board_) {
      throw std::runtime_error("Game boards are not equal at the beginning");
    }
  }
  ~GameBoardTester() = default;

  void run(int rounds, std::vector<void (*)(AbstractGameBoard*)> god_funcs) {
    for (int i = 0; i < rounds; i++) {
      for (auto god_func : god_funcs) {
        god_func(game_board_);
        god_func(alternative_game_board_);
        if (*game_board_ != *alternative_game_board_) {
          throw std::runtime_error("God function changed the game board");
        }
      }
      auto begin = std::chrono::high_resolution_clock::now();
      game_board_->update();
      auto end = std::chrono::high_resolution_clock::now();
      game_board_cpu_time_ +=
          std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
              .count();

      begin = std::chrono::high_resolution_clock::now();
      alternative_game_board_->update();
      end = std::chrono::high_resolution_clock::now();
      alternative_game_board_cpu_time_ +=
          std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
              .count();

      if (*game_board_ != *alternative_game_board_) {
        throw std::runtime_error("Game boards are not equal at the round " +
                                 std::to_string(i));
      }
    }
  }

  std::string report_cpu_time() {
    return "Game board CPU time: " + std::to_string(game_board_cpu_time_) +
           "ms, Alternative game board CPU time: " +
           std::to_string(alternative_game_board_cpu_time_) + "ms.";
  }

 private:
  AbstractGameBoard* game_board_;
  AbstractGameBoard* alternative_game_board_;
  // accumulate CPU time for game_board_ and alternative_game_board_
  // and compare them at the end
  uint64_t game_board_cpu_time_{0};
  uint64_t alternative_game_board_cpu_time_{0};
};
