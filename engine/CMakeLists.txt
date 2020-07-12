cmake_minimum_required(VERSION 3.10)
project(dempsta)

set(CMAKE_CXX_STANDARD 17)

# Include cmake util functions
include(${CMAKE_CURRENT_SOURCE_DIR}/utils/cmake/utils.cmake)

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

    if (BUILD_DOC)
        find_package(Doxygen)
        if (DOXYGEN_FOUND)
            # set input and output files
            set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
            set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

            # request to configure the file
            configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
            message("Doxygen build started for Dempsta Engine")

            # note the option ALL which allows to build the docs together with the application
            add_custom_target(doc_dempsta ALL
                COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "Generating API documentation with Doxygen"
                VERBATIM )
        else (DOXYGEN_FOUND)
            message("Doxygen need to be installed to generate the doxygen documentation")
        endif (DOXYGEN_FOUND)
    endif(BUILD_DOC)
endif ()

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
# Add stb_image
add_subdirectory(external/stb_image)
# Add lua
add_subdirectory(external/lua)
# Add EnTT
add_subdirectory(external/entt)

target_include_directories(dempsta PUBLIC include)

target_link_libraries(dempsta
        PUBLIC
        spdlog
        glfw
        glad
        glm
        imgui
        stb_image
        lua
        entt)

set_target_properties(dempsta PROPERTIES INSTALL_RPATH $ORIGIN)

if (MSVC)
    target_compile_options(dempsta PRIVATE /W4)
else ()
    target_compile_options(dempsta PRIVATE -Wall -pedantic)
endif ()

install(TARGETS dempsta DESTINATION lib)
file(GLOB_RECURSE INCLUDES include/*.h)
install(FILES ${INCLUDES} DESTINATION include/dempsta)
