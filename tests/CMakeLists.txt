project("Xli Tests")

cmake_minimum_required(VERSION 2.6)

include(${PROJECT_SOURCE_DIR}/../Xli.cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

add_executable(XliTest core/test.cpp core/Unicode_test.cpp core/DateTime_test.cpp main.cpp)
target_link_libraries(XliTest Xli)
