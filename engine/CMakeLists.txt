cmake_minimum_required(VERSION 3.10)
project(dempsta)

set(CMAKE_CXX_STANDARD 17)

# Include cmake util functions
include(${CMAKE_CURRENT_SOURCE_DIR}/utils/cmake/utils.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

message("Building Dempsta Engine...")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: Debug Release."
            FORCE)
endif ()

if (BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
    set(SPDLOG_BUILD_SHARED ON CACHE INTERNAL "")
else ()
    set(LIB_TYPE STATIC)
    set(SPDLOG_BUILD_SHARED OFF CACHE INTERNAL "")
endif ()

if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("-- Build type: Debug")
    message("Adding Debug flag...")
    add_definitions(-DNDEBUG)
else ()
    message("-- Build type: Release")
    add_definitions(-UNDEBUG)
endif ()

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(dempsta ${LIB_TYPE} ${SOURCES})

# Add spdlog
#cmake_policy(SET CMP0072 NEW)
set(SPDLOG_BUILD_TESTING OFF CACHE INTERNAL "")
set(SPDLOG_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(external/spdlog)

# Add glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# Add glad
add_subdirectory(external/glad)

# Add imgui
add_subdirectory(external/imgui)

# Add glm
add_subdirectory(external/glm)

# Add stb_image
add_subdirectory(external/stb_image)

target_include_directories(dempsta PUBLIC include)

target_link_libraries(dempsta
        PUBLIC
        spdlog
        glfw
        glad
        glm
        imgui
        stb_image)

set_target_properties(dempsta PROPERTIES INSTALL_RPATH $ORIGIN)

if (MSVC)
    target_compile_options(dempsta PRIVATE /W4 /WX)
else ()
    target_compile_options(dempsta PRIVATE -Wall -pedantic)
endif ()

install(TARGETS dempsta DESTINATION lib)
file(GLOB_RECURSE INCLUDES include/*.h)
install(FILES ${INCLUDES} DESTINATION include/dempsta)
