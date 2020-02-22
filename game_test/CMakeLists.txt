cmake_minimum_required(VERSION 3.10)
project(game_test)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.h src/*.cpp)

add_executable(game_test ${SOURCES})

target_link_libraries(game_test PRIVATE dempsta)
