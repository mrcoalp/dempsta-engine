# Download SFML if doesn't exist
function(downloadSFML)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML-2.5.1")
        file(DOWNLOAD "https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz" ${CMAKE_CURRENT_SOURCE_DIR}/sfml.tar.gz
                SHOW_PROGRESS
                TIMEOUT 60  # seconds
                TLS_VERIFY ON
                )

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external)

        execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xzvf ${CMAKE_CURRENT_SOURCE_DIR}/sfml.tar.gz
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external)

        file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/sfml.tar.gz)
    endif ()
endfunction()

# Download GLEW if doesn't exist
function(downloadGLEW)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.1.0")
        file(DOWNLOAD
                "https://downloads.sourceforge.net/project/glew/glew/2.1.0/glew-2.1.0.tgz?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fglew%2Ffiles%2Fglew%2F2.1.0%2Fglew-2.1.0.tgz%2Fdownload&ts=1581794893"
                ${CMAKE_CURRENT_SOURCE_DIR}/glew.tgz
                SHOW_PROGRESS
                TIMEOUT 60  # seconds
                TLS_VERIFY ON
                )

        file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external)

        execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xzvf ${CMAKE_CURRENT_SOURCE_DIR}/glew.tgz
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external)

        file(REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/glew.tgz)
    endif ()
endfunction()
