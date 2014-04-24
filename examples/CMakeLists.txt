project("Xli Examples")

cmake_minimum_required(VERSION 2.6)

include(${PROJECT_SOURCE_DIR}/../Xli.cmake)

if (MSVC)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build Configurations (forced)" FORCE)

endif()

add_subdirectory(GLWindow)
add_subdirectory(AssetLoading)
add_subdirectory(HttpTest)
