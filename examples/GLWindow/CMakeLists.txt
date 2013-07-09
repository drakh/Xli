project("GLWindow")

cmake_minimum_required(VERSION 2.6)

# Path to Xli root directory
set(XLI_DIR ${PROJECT_SOURCE_DIR}/../..)

include_directories(${XLI_DIR}/include)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

	link_directories(${XLI_DIR}/lib/OSX/x86)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-framework,OpenGL")

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

	if (CMAKE_SIZEOF_VOID_P EQUAL 8)
		link_directories(${XLI_DIR}/lib/linux/x86_64)
	else()
		link_directories(${XLI_DIR}/lib/linux/x86_32)
	endif()

elseif (MSVC)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" INTERNAL)
	link_directories("${XLI_DIR}/lib/vs2012/$(PlatformShortName)")
	
else()

	link_directories(${XLI_DIR}/lib)

endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXECUTABLE_OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXECUTABLE_OUTPUT_PATH})

add_executable(GLWindow src/Main.cpp)
target_link_libraries(GLWindow Xli XliMain)
