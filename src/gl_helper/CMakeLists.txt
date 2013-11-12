if (APPLE)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,OpenGL")
endif()

add_library(XliGLHelper GLHelper.cpp)
target_link_libraries(XliGLHelper Xli XliGL XliMedia)
