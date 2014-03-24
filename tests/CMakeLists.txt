project("Xli Tests")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

cmake_minimum_required(VERSION 2.6)

include(${PROJECT_SOURCE_DIR}/../Xli.cmake)

if (MSVC)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build Configurations (forced)" FORCE)

endif()

add_executable(XLI_Test core/test.cpp core/Unicode_test.cpp main.cpp)
target_link_libraries(XLI_Test Xli)
