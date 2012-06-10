set(Sources
	GLFont.cpp
	GLFontRenderer.cpp
	GLFramebuffer.cpp
	GLHelpers.cpp
	GLProgram.cpp
	GLTexture2D.cpp
	)

if (WIN32)

	add_definitions(-DGLEW_STATIC)

	include_directories(3rdparty/glew/include)
	set(Sources ${Sources} 3rdparty/glew/src/glew.c)

	if (XLI_SDL2)

		set(Sources ${Sources} SDL2Context.cpp)

		include_directories(${PROJECT_SOURCE_DIR}/src/platform/sdl2/SDL2-win32/include)
		link_directories(${PROJECT_SOURCE_DIR}/src/platform/sdl2/SDL2-win32/lib)

	else()
		
		set(Sources ${Sources} WGLContext.cpp)
	
	endif()

elseif (APPLE)

	add_definitions(-DGLEW_STATIC)

	include_directories(3rdparty/glew/include)
	set(Sources ${Sources} 3rdparty/glew/src/glew.c)

	set(Sources ${Sources} SDL2Context.cpp)
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-framework,OpenGL")
	
elseif (UNIX)

	#if (XLI_SDL2)

		set(Sources ${Sources} SDL2Context.cpp)
		#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} `sdl2-config --libs`")

	#else()

		#set(Sources ${Sources} GLXContext.cpp)

	#endif()
	
endif ()

add_library(XliGL ${Sources})

target_link_libraries(XliGL Xli XliMedia XliGraphics XliPlatform)

if (UNIX AND NOT APPLE)

	target_link_libraries(XliGL SDL2 GLEW GL)

endif()
