set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(GLWindow src/Main.cpp)
target_link_libraries(GLWindow Xli XliMain GL)
