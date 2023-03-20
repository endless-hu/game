cmake_minimum_required(VERSION 3.0)
project(game_of_life)

set(CMAKE_CXX_STANDARD 11)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Specify location of SDL2_ttf library and header files
set(SDL2_TTF_INCLUDE_DIRS "/usr/include/SDL2")
set(SDL2_TTF_LIBRARIES "/usr/lib/x86_64-linux-gnu/libSDL2_ttf.so")

# Add source files and link libraries
add_executable(game_of_life src/main.cc src/game_board.cc)
# Link against SDL2 and SDL2_ttf libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})