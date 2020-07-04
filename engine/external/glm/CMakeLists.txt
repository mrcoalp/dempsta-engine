cmake_minimum_required(VERSION 3.10)
project(glm)

add_library(glm INTERFACE)

target_include_directories(glm INTERFACE .)

install(TARGETS glm DESTINATION lib)
file(GLOB_RECURSE INCLUDES glm/*.h glm/*.hpp)
install(FILES ${INCLUDES} DESTINATION include/glm)
