cmake_minimum_required(VERSION 3.10)
project(glm)

add_library(glm INTERFACE)

target_include_directories(glm INTERFACE .)

install(TARGETS glm DESTINATION lib)
file(GLOB_RECURSE INCLUDES ./*.h)
install(FILES ${INCLUDES} DESTINATION include/glm)
