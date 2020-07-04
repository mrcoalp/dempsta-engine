cmake_minimum_required(VERSION 3.10)
project(spdlog)

add_library(spdlog INTERFACE)

target_include_directories(spdlog INTERFACE .)

install(TARGETS spdlog DESTINATION lib)
file(GLOB_RECURSE INCLUDES spdlog/*.h)
install(FILES ${INCLUDES} DESTINATION include/spdlog)
