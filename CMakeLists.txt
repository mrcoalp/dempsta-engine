cmake_minimum_required(VERSION 3.10)
project(game_test)

set(CMAKE_CXX_STANDARD 17)

# Handle rpath
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")
#set(CMAKE_SKIP_BUILD_RPATH FALSE)
#set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
#set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
if (WIN32)
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
else ()
    set(BUILD_SHARED_LIBS ON CACHE INTERNAL "")
endif ()

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

install(TARGETS game_test
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib)
