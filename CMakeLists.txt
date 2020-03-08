cmake_minimum_required(VERSION 3.10)
project(game_test)

set(CMAKE_CXX_STANDARD 17)

message("Building game...")

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    message("-- Build type: Debug")
    message("Adding Debug flag...")
    add_definitions(-DNDEBUG)
else ()
    message("-- Build type: Release")
    add_definitions(-UNDEBUG)
endif ()

add_subdirectory(engine)

file(GLOB_RECURSE SOURCES src/*.h src/*.cpp)

add_executable(game_test ${SOURCES})

target_link_libraries(game_test PRIVATE dempsta)
