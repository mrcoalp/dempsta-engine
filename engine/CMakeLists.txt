cmake_minimum_required(VERSION 3.10)
project(dempsta)

set(CMAKE_CXX_STANDARD 17)

# Include cmake util functions
include(${CMAKE_CURRENT_SOURCE_DIR}/utils/cmake/utils.cmake)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)
add_library(dempsta SHARED ${SOURCES})

# Add spdlog
#cmake_policy(SET CMP0072 NEW)
set(SPDLOG_BUILD_SHARED ON CACHE INTERNAL "")
add_subdirectory(external/spdlog)

target_include_directories(dempsta
        PRIVATE
        src
        PUBLIC
        include
        )

target_link_libraries(dempsta spdlog)
