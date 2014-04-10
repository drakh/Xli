if (IOS)
	include_directories(${PROJECT_SOURCE_DIR}/src/core/sdl2/3rdparty/SDL2/include)
endif()

add_library(XliMain STATIC XliMain.cpp)
