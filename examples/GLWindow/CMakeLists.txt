set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if (APPLE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-framework,OpenGL")
endif()

add_executable(GLWindow Main.cpp)
target_link_libraries(GLWindow Xli XliGL XliMain XliPlatform)

if (XLI_PLATFORM_LINUX AND NOT XLI_ARCH_ARM)
    target_link_libraries(GLWindow GL GLEW)
endif()
