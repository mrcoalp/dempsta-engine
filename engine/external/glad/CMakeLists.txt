cmake_minimum_required(VERSION 3.10)
project(glad)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

if (BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else ()
    set(LIB_TYPE STATIC)
endif ()

file(GLOB_RECURSE SOURCES src/*.c)
add_library(glad ${LIB_TYPE} ${SOURCES})

target_include_directories(glad PUBLIC include)

install(TARGETS glad DESTINATION lib)
file(GLOB_RECURSE INCLUDES include/*.h)
install(FILES ${INCLUDES} DESTINATION include/glad)
