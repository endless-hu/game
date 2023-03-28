cmake_minimum_required(VERSION 3.0)
project(game_of_life)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Specify location of SDL2_ttf library and header files
set(SDL2_TTF_INCLUDE_DIRS "/usr/include/SDL2")
set(SDL2_TTF_LIBRARIES "/usr/lib/x86_64-linux-gnu/libSDL2_ttf.so")

# Set the build type to Debug or Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Set the compiler flags, use AddressSanitizer for Debug builds
if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -fsanitize=address")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
endif()

# Add source files and link libraries
# Add all .cc files under src/
file(GLOB_RECURSE SOURCES "src/*.cc")
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against SDL2 and SDL2_ttf libraries
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})

# Configure all ctests under test/
# The test sources should include the header files and 
# their corresponding source files in src/
enable_testing()
# Add the src/ directory to the list of directories to search for include files
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE TEST_SOURCES "test/*.cc")
foreach(TEST_SOURCE ${TEST_SOURCES})
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_SOURCE})
  # Add header and source files from src/ to test/
  target_sources(${TEST_NAME} PRIVATE
      ${PROJECT_SOURCE_DIR}/src/bit_map.hh
      ${PROJECT_SOURCE_DIR}/src/game_board.hh
      ${PROJECT_SOURCE_DIR}/src/game_board.cc
  )
  target_include_directories(${TEST_NAME} PUBLIC ${SDL2_TTF_INCLUDE_DIRS})
  target_link_libraries(${TEST_NAME} ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
