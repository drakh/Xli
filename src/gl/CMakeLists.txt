if (WIN32)

 
elseif (UNIX)

    set(Sources sdl2/SDL2GLContext.cpp)

	add_definitions(-DXLI_HAS_GLEW)

	if (APPLE)

	    set(Sources ${Sources} 3rdparty/glew/src/glew.c)
	    add_definitions(-DGLEW_STATIC)
	    include_directories(${PROJECT_SOURCE_DIR}/src/gl/3rdparty/glew/include)
	    include_directories(${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2/include)
	    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,OpenGL")

	else()

		add_definitions(`sdl2-config --cflags`)

	endif()

endif()

add_library(XliGL ${Sources})
target_link_libraries(XliGL Xli)

if (UNIX AND NOT APPLE AND NOT WIN32)
    target_link_libraries(XliGL SDL2 GL GLEW)
endif()
