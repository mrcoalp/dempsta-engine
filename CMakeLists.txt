cmake_minimum_required(VERSION 3.10)
project(dempsta_editor)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_DOC "Build documentation" OFF)

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

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("-- Build type: Debug")
    message("Adding Debug flag...")
    add_definitions(-DNDEBUG)
else ()
    message("-- Build type: Release")
    add_definitions(-UNDEBUG)
endif ()

if (BUILD_DOC)
    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        add_subdirectory(docs)
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif(BUILD_DOC)

add_subdirectory(engine)

file(GLOB_RECURSE SOURCES editor/*.h editor/*.cpp)

add_executable(dempsta_editor ${SOURCES})

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

target_link_libraries(dempsta_editor PRIVATE dempsta)
# Copy assets folder to binary destination
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Install custom instructions
install(TARGETS dempsta_editor
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib)
# Copy assets folder to installation dir root
install(DIRECTORY assets DESTINATION .)
