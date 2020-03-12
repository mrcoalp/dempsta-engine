cmake_minimum_required(VERSION 3.10)
project(glad)

file(GLOB_RECURSE SOURCES src/*.c)
add_library(glad SHARED ${SOURCES})

target_include_directories(glad PUBLIC include)

install(TARGETS glad DESTINATION lib)
file(GLOB_RECURSE INCLUDES include/*.h)
install(FILES ${INCLUDES} DESTINATION include)
