# Freetype GL - A C OpenGL Freetype engine
#
# Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
# file `LICENSE` for more details.

cmake_minimum_required(VERSION 2.8.12)

project(freetype-gl LANGUAGES C CXX)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules"
    "${CMAKE_MODULE_PATH}"
)

message(STATUS "Building for ${CMAKE_SYSTEM_NAME} target system")
message(STATUS "Building with ${CMAKE_C_COMPILER_ID} compiler")

option(freetype-gl_WITH_GLAD "Use the GLAD gl loader" ON)

include(RequireIncludeFile)
include(RequireFunctionExists)
include(CheckLibraryExists)
include(CheckSymbolExists)

require_include_file(stdbool.h HAVE_STDBOOL_H)
require_include_file(stdint.h HAVE_STDINT_H)
require_include_file(math.h HAVE_MATH_H)

check_library_exists(m cos "" HAVE_MATH_LIBRARY)

if(HAVE_MATH_LIBRARY)
    list(APPEND CMAKE_REQUIRED_LIBRARIES m)
    set(MATH_LIBRARY m)
endif()

require_function_exists(cos HAVE_COS)
require_function_exists(fabs HAVE_FABS)
require_function_exists(floor HAVE_FLOOR)
require_function_exists(fmod HAVE_FMOD)
require_function_exists(pow HAVE_POW)
require_function_exists(roundf HAVE_ROUNDF)
require_function_exists(round HAVE_ROUND)
require_function_exists(sin HAVE_SIN)
require_function_exists(sqrt HAVE_SQRT)
require_function_exists(tan HAVE_TAN)

check_symbol_exists(M_PI math.h HAVE_M_PI)

if(NOT HAVE_M_PI)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -D_USE_MATH_DEFINES)
    unset(HAVE_M_PI CACHE)

    check_symbol_exists(M_PI math.h HAVE_M_PI)

    if(NOT HAVE_M_PI)
        message(FATAL_ERROR "`M_PI` not defined in `math.h`.")
    else()
        add_definitions(-D_USE_MATH_DEFINES)
    endif()
endif()

find_package(OpenGL REQUIRED)
# find_package(Freetype REQUIRED)

if(freetype-gl_WITH_GLAD)
    message("freetype-gl - including GLAD in ${GLAD_INCLUDE_PATH}")
    add_definitions(-DGL_WITH_GLAD)
endif()

set(FREETYPE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/freetype/include" CACHE STRING "" FORCE)

include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${GLAD_INCLUDE_PATH}
)

if(MSVC)
    # _CRT_NONSTDC_NO_DEPRECATE -> remove warning C4996
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
endif(MSVC)

add_subdirectory(freetype)

file(GLOB_RECURSE SOURCES src/*.c)
add_library(freetype-gl ${LIB_TYPE} ${SOURCES})

target_link_libraries(freetype-gl PUBLIC freetype)

target_include_directories(freetype-gl PUBLIC include)

install(TARGETS freetype-gl DESTINATION lib)
file(GLOB_RECURSE INCLUDES include/*.h)
install(FILES ${INCLUDES} DESTINATION include/freetype-gl)
