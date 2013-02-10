project("GLWindow")

cmake_minimum_required(VERSION 2.6)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# Path to Xli root directory
set(XLI_DIR ${PROJECT_SOURCE_DIR}/../..)

include_directories(${XLI_DIR}/include)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

	link_directories(${XLI_DIR}/lib/osx/x86)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-framework,OpenGL")

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		link_directories(${XLI_DIR}/lib/linux/x86_64)
	else()
		link_directories(${XLI_DIR}/lib/linux/x86_32)
	endif()

else()

	link_directories(${XLI_DIR}/lib)

endif()

add_executable(GLWindow Main.cpp)

target_link_libraries(GLWindow Xli XliMain XliMedia XliAudio)
