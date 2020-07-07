set(BINARY ${CMAKE_PROJECT_NAME}_tst)

set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

add_executable(${BINARY} test.cpp)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC dempsta)

# Copy scripts folder to binary destination
file(COPY scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Install custom instructions
install(TARGETS ${BINARY}
        RUNTIME DESTINATION ./test)
# Copy scripts folder to installation dir root
install(DIRECTORY scripts DESTINATION ./test)
