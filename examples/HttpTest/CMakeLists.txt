set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if (APPLE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-framework,OpenGL")
endif()

add_executable(HttpTest src/Main.cpp)
target_link_libraries(HttpTest Xli XliGL XliMain XliHttpClient XliPlatform)

if (UNIX AND NOT APPLE AND NOT WIN32)
    target_link_libraries(HttpTest GL GLEW)
endif()

