cmake_minimum_required(VERSION 3.10)
project(dempsta)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_DOC "Build documentation" OFF)

message("Building Dempsta Engine...")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: Debug Release."
            FORCE)
endif ()

if (BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
else ()
    set(LIB_TYPE STATIC)
endif ()

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
endif (BUILD_DOC)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_library(dempsta ${LIB_TYPE} ${SOURCES})

# Add spdlog
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
# Add moon
set(MOON_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(external/moon)
# Add freetype-gl
find_package(Freetype)
if (NOT FREETYPE_FOUND)
    message("Freetype not found, trying to build it...")
    set(FREETYPE_INCLUDE_DIRS "")
    set(FREETYPE_LIBRARIES freetype)
    add_subdirectory(external/freetype)
endif ()
# Add OpenAL
set(ALSOFT_DLOPEN OFF CACHE BOOL "Check for the dlopen API for loading optional libs" FORCE)
set(ALSOFT_UTILS OFF CACHE BOOL "Build and install utility programs" FORCE)
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Build and install example programs" FORCE)
set(ALSOFT_TESTS OFF CACHE BOOL "Build and install test programs" FORCE)
set(ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "Install alsoft.conf sample configuration file" FORCE)
set(ALSOFT_INSTALL_HRTF_DATA OFF CACHE BOOL "Install HRTF definition files" FORCE)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "Install AmbDec preset files" FORCE)
set(CMAKE_INSTALL_BINDIR .)
add_subdirectory(external/openal-soft)

target_include_directories(dempsta PUBLIC include external/header_only ${FREETYPE_INCLUDE_DIRS})

target_link_libraries(dempsta PUBLIC spdlog glfw glad glm imgui moon ${FREETYPE_LIBRARIES} OpenAL)

set_target_properties(dempsta PROPERTIES INSTALL_RPATH $ORIGIN)

if (MSVC)
    target_compile_options(dempsta PRIVATE /W4)
else ()
    target_compile_options(dempsta PRIVATE -Wall -pedantic)
endif ()

install(TARGETS dempsta DESTINATION lib)
file(GLOB_RECURSE INCLUDES include/*.h)
install(FILES ${INCLUDES} DESTINATION include/dempsta)
