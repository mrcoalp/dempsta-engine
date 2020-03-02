cmake_minimum_required(VERSION 3.10)
project(dempsta)

set(CMAKE_CXX_STANDARD 17)

# Include cmake util functions
include(${CMAKE_CURRENT_SOURCE_DIR}/utils/cmake/utils.cmake)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(dempsta SHARED ${SOURCES})

# Add spdlog
#cmake_policy(SET CMP0072 NEW)
set(SPDLOG_BUILD_SHARED ON CACHE INTERNAL "")
set(SPDLOG_BUILD_TESTING OFF CACHE INTERNAL "")
add_subdirectory(external/spdlog)

# Add glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE INTERNAL "")
add_subdirectory(external/glfw)

# Add OpenGL
#set(OpenGL_GL_PREFERENCE GLVND)
#find_package(OpenGL REQUIRED)

# Add glad
add_subdirectory(external/glad)

# Add imgui
add_subdirectory(external/imgui)

target_include_directories(dempsta PUBLIC include)

target_link_libraries(dempsta spdlog glfw glad imgui)
