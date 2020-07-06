cmake_minimum_required(VERSION 3.10)
project(entt)

add_library(entt INTERFACE)

target_include_directories(entt INTERFACE .)

install(TARGETS entt DESTINATION lib)
file(GLOB_RECURSE INCLUDES ./*.h ./*.hpp)
install(FILES ${INCLUDES} DESTINATION include/entt)
