cmake_minimum_required(VERSION 3.10)
project(stb_image)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

if (BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else ()
    set(LIB_TYPE STATIC)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(stb_image ${LIB_TYPE} ${SOURCES})

target_include_directories(stb_image PUBLIC include)

install(TARGETS stb_image DESTINATION lib)
file(GLOB_RECURSE INCLUDES include/*.h)
install(FILES ${INCLUDES} DESTINATION include/stb_image)
