cmake_minimum_required(VERSION 3.0)
project(game_of_life)

set(CMAKE_CXX_STANDARD 11)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Add source files and link libraries
add_executable(game_of_life src/main.cc src/game_board.cc)
target_link_libraries(game_of_life ${SDL2_LIBRARIES})